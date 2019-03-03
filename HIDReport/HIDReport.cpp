#include "stdafx.h"

#include <hidsdi.h>
#include <hidpi.h>
#include <SetupAPI.h>
#include <initguid.h>
#include <usbiodef.h>

#define HID_IMPORT_EXPORT _declspec(dllexport) _stdcall 
#include "HIDReport.h"


#pragma once

#ifndef HID_IMPORT_EXPORT
#define HID_IMPORT_EXPORT _declspec(dllimport) _stdcall 
#endif

namespace
{
    class HandleFactory
    {
    public:
        static int GetNextHandle()
        {
            return ++lastHandle;
        }
    private:
        static int lastHandle;
    };

    int HandleFactory::lastHandle = 0;

    class FileHandle
    {
    public:
        FileHandle() : m_handle(INVALID_HANDLE_VALUE)
        {
        }

        FileHandle(HANDLE handle) : m_handle(handle)
        {
        }

        FileHandle(FileHandle&) = delete;
        FileHandle(FileHandle&& a)
        {
            m_handle = a.m_handle;
            a.m_handle = INVALID_HANDLE_VALUE;
        }

        FileHandle& operator =(HANDLE handle)
        {
            if (m_handle != INVALID_HANDLE_VALUE)
            {
                CloseHandle(m_handle);
            }

            m_handle = handle;
            return *this;
        }

        operator bool()
        {
            return (m_handle != INVALID_HANDLE_VALUE);
        }

        operator HANDLE()
        {
            return m_handle;
        }

        ~FileHandle()
        {
            if (m_handle != INVALID_HANDLE_VALUE)
            {
                CloseHandle(m_handle);
            }
        }

    private:
        HANDLE m_handle = INVALID_HANDLE_VALUE;
    };

    class PreparsedData
    {
    public:
        void Open(HANDLE deviceObject)
        {
            HidD_GetPreparsedData(deviceObject, &preparsedData);
        }

        PreparsedData() {}

        PreparsedData(PreparsedData&) = delete;

        PreparsedData(PreparsedData&& a)
        {
            preparsedData = a.preparsedData;
            a.preparsedData = nullptr;
        }

        ~PreparsedData()
        {
            if (preparsedData)
            {
                HidD_FreePreparsedData(preparsedData);
            }
        }

        operator bool()
        {
            return (preparsedData != nullptr);
        }

        PHIDP_PREPARSED_DATA Get()
        {
            return preparsedData;
        }

    private:
        PHIDP_PREPARSED_DATA preparsedData = nullptr;
    };

    class Device
    {
    public:
        Device(const WCHAR* deviceName) : m_deviceName(deviceName)
        {
        }

        ~Device() = default;

        Device(Device&) = delete;
        Device(Device&& a) = default;

        void Close()
        {
            UnInit();
        }

        int CanBeOpened()
        {
            Init();
            return m_hidDeviceObject ? 1 : 0;
        }

        const std::wstring& GetManufacturer()
        {
            Init();
            return m_mfgr;
        }

        const std::wstring& GetProduct()
        {
            Init();
            return m_product;
        }

        int GetReportCount(int type)
        {
            Init();
            if (type >= 0 && type <= 2)
            {
                return m_reportCount[type];
            }
            return 0;
        }

        int GetVID()
        {
            Init();
            return m_vid;
        }

        int GetPID()
        {
            Init();
            return m_pid;
        }

    private:
        void Init()
        {
            if (m_isInitialized) return;
            m_hidDeviceObject = CreateFile(m_deviceName.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr /*lpSecurityAttributes*/, 
                OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr /*hTemplateFile*/);

            if (m_hidDeviceObject)
            {
                m_preparsedData.Open(m_hidDeviceObject);

                HIDD_ATTRIBUTES attributes{};
                attributes.Size = sizeof(attributes);
                HidD_GetAttributes(m_hidDeviceObject, &attributes);

                m_vid = attributes.VendorID;
                m_pid = attributes.ProductID;

                const int USB_MAX_STRING = 127; // 126 plus a null terminator; max USB string length is 126.
                wchar_t tempString[USB_MAX_STRING] = {0};

                HidD_GetManufacturerString(m_hidDeviceObject, tempString, sizeof(tempString));
                m_mfgr = tempString;

                tempString[0] = 0;
                HidD_GetProductString(m_hidDeviceObject, tempString, sizeof(tempString));
                m_product = tempString;

                ParseReportInformation();

                m_isInitialized = true;
            }
        }

        void ParseReportInformation()
        {
            // Get the top-level capabilities for the device
            HidP_GetCaps(m_preparsedData.Get(), &m_topLevelCaps);
            
            // Get all of the collections and initialize their types to 'unused'
            unsigned long length = m_topLevelCaps.NumberLinkCollectionNodes;
            m_collectionNodes = std::make_unique<HIDP_LINK_COLLECTION_NODE[]>(length);
            HidP_GetLinkCollectionNodes(m_collectionNodes.get(), &length, m_preparsedData.Get());

            m_collectionTypes.insert(m_collectionTypes.cbegin(), length, 0);

            // Get the button capabilities for all of the report types
            GetButtonCaps(HidP_Input, m_topLevelCaps.NumberInputButtonCaps);
            GetButtonCaps(HidP_Output, m_topLevelCaps.NumberOutputButtonCaps);
            GetButtonCaps(HidP_Feature, m_topLevelCaps.NumberFeatureButtonCaps);

            // Get the value capabilities for all of the report types
            GetValueCaps(HidP_Input, m_topLevelCaps.NumberInputValueCaps);
            GetValueCaps(HidP_Output, m_topLevelCaps.NumberOutputValueCaps);
            GetValueCaps(HidP_Feature, m_topLevelCaps.NumberFeatureValueCaps);

            for (int i = 0; i < 3; ++i)
            {
                if (m_reportCount[i] == 0 && !(m_collectionTypes[0] & (1 << i)))
                {
                    m_reportCount[i] = -1;
                }
            }
        }

        void UpdateLinkCollectionTypes(unsigned short collectionID, HIDP_REPORT_TYPE type)
        {
            int flag = 1 << type;

            do
            {
                m_collectionTypes[collectionID] |= flag;
                collectionID = m_collectionNodes[collectionID].Parent;
            } while (collectionID);
        }

        void GetButtonCaps(HIDP_REPORT_TYPE type, unsigned short count)
        {
            if (!count) return;
            m_buttonCaps[type] = std::make_unique<HIDP_BUTTON_CAPS[]>(count);
            HidP_GetButtonCaps(type, m_buttonCaps[type].get(), &count, m_preparsedData.Get());

            for (size_t i = 0; i < count; ++i)
            {
                auto& buttonCap{ m_buttonCaps[type][i] };
                if (buttonCap.ReportID > m_reportCount[type])
                {
                    m_reportCount[type] = buttonCap.ReportID;
                }
                UpdateLinkCollectionTypes(buttonCap.LinkCollection, type);
            }
        }

        void GetValueCaps(HIDP_REPORT_TYPE type, unsigned short count)
        {
            if (!count) return;
            m_valueCaps[type] = std::make_unique<HIDP_VALUE_CAPS[]>(count);
            HidP_GetValueCaps(type, m_valueCaps[type].get(), &count, m_preparsedData.Get());

            for (size_t i = 0; i < count; ++i)
            {
                auto& valueCap{ m_valueCaps[type][i] };
                if (valueCap.ReportID > m_reportCount[type])
                {
                    m_reportCount[type] = valueCap.ReportID;
                }
                UpdateLinkCollectionTypes(valueCap.LinkCollection, type);
            }
        }

        void UnInit()
        {
            if (!m_isInitialized) return;

            m_isInitialized = false;
        }

        std::wstring m_deviceName;
        bool m_isInitialized = false;

        std::wstring m_mfgr;
        std::wstring m_product;

        int m_vid = 0;
        int m_pid = 0;

        int m_reportCount[3] = {};

        FileHandle m_hidDeviceObject;
        PreparsedData m_preparsedData;

        HIDP_CAPS m_topLevelCaps = {};
        std::unique_ptr<HIDP_LINK_COLLECTION_NODE[]> m_collectionNodes;
        std::unique_ptr<HIDP_BUTTON_CAPS[]> m_buttonCaps[3];
        std::unique_ptr<HIDP_VALUE_CAPS[]> m_valueCaps[3];

        const int COL_INPUT = 1 << HidP_Input;
        const int COL_OUTPUT = 1 << HidP_Output;
        const int COL_FEATURE = 1 << HidP_Feature;
        std::vector<int> m_collectionTypes;

    };

    class DeviceStore
    {
    public:
        DeviceStore()
        {
            GUID hidGuid{};
            HidD_GetHidGuid(&hidGuid);

            auto classDevs = SetupDiGetClassDevsW(&hidGuid, nullptr /*Enumerator*/, nullptr /*hwndParent*/, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
            if (classDevs == INVALID_HANDLE_VALUE) return;

            DWORD memberIndex = 0;
            SP_DEVICE_INTERFACE_DATA deviceInterfaceData{};
            deviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
            while (SetupDiEnumDeviceInterfaces(classDevs, nullptr /*DeviceInfoData*/, &hidGuid, memberIndex++, &deviceInterfaceData))
            {
                // Get required size
                DWORD requiredSize = 0;
                SetupDiGetDeviceInterfaceDetail(classDevs, &deviceInterfaceData, nullptr /*DeviceInterfaceDetailData*/, 0, &requiredSize, nullptr /*DeviceInfoData*/);
                if (requiredSize)
                {
                    auto deviceInterfaceDetailDataBuffer = std::make_unique<unsigned char[]>(requiredSize);
                    auto deviceInterfaceDetailData = reinterpret_cast<SP_DEVICE_INTERFACE_DETAIL_DATA*>(deviceInterfaceDetailDataBuffer.get());
                    deviceInterfaceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
                    if (SetupDiGetDeviceInterfaceDetail(classDevs, &deviceInterfaceData, deviceInterfaceDetailData, requiredSize, &requiredSize, nullptr /*DeviceInfoData*/))
                    {
                        m_handles.push_back(HandleFactory::GetNextHandle());
                        m_devices.emplace(std::make_pair(m_handles.back(), Device(deviceInterfaceDetailData->DevicePath)));
                    }
                }
            }
            int error = GetLastError();
        }

        int GetCount()
        {
            return static_cast<int>(m_handles.size());
        }

        int GetHandle(int index)
        {
            return m_handles[index];
        }

        bool HasDevice(int handle)
        {
            return (m_devices.count(handle) != 0);
        }

        Device& GetDevice(int handle)
        {
            auto iter = m_devices.find(handle);
            if (iter != m_devices.end())
            {
                return iter->second;
            }
            throw std::runtime_error("Device Not Found");
        }
    private:
        std::vector<int> m_handles;
        std::map<int, Device> m_devices;
    };

    std::map<int, DeviceStore> devices;

    Device& LookupDevice(int handle)
    {
        for (auto&& [first, second] : devices)
        {
            if (second.HasDevice(handle))
            {
                return second.GetDevice(handle);
            }
        }
        throw std::runtime_error("Invalid Device Handle");
    }

    int DoStringReturn(wchar_t* string, int stringSize, std::wstring& cppString)
    {
        if (string)
        {
            wcscpy_s(string, stringSize, cppString.c_str());
            return static_cast<int>(wcslen(string) + 1);
        }
        else
        {
            return static_cast<int>(cppString.length() + 1);
        }
    }
}

extern "C"
{
    int HID_IMPORT_EXPORT Devices_Open()
    {
        int handle = HandleFactory::GetNextHandle();
        devices.emplace(std::make_pair(handle, DeviceStore()));
        return handle;
    }

    int HID_IMPORT_EXPORT Devices_Count(int handle)
    {
        auto iter = devices.find(handle);
        if (iter != devices.end())
        {
            return iter->second.GetCount();
        }
        return 0;
    }

    int HID_IMPORT_EXPORT Devices_OpenDevice(int handle, int index)
    {
        auto iter = devices.find(handle);
        if (iter != devices.end())
        {
            return iter->second.GetHandle(index);
        }
        return 0;
    }

    void HID_IMPORT_EXPORT Devices_Close(int handle)
    {
        devices.erase(handle);
    }


    void HID_IMPORT_EXPORT Device_Close(int handle)
    {
        LookupDevice(handle).Close();
    }

    int HID_IMPORT_EXPORT Device_Opened(int handle)
    {
        return LookupDevice(handle).CanBeOpened();
    }

    int HID_IMPORT_EXPORT Device_ReportCount(int handle, int type)
    {
        return LookupDevice(handle).GetReportCount(type);
    }

    int HID_IMPORT_EXPORT Device_OpenReportCollection(int handle, int type, int id)
    {
        return 0; // LookupDevice(handle).OpenReportCollection(type, id);
    }

    int HID_IMPORT_EXPORT Device_Manufacturer(int handle, wchar_t* name, int nameSize)
    {
        auto deviceName = LookupDevice(handle).GetManufacturer();
        return DoStringReturn(name, nameSize, deviceName);
    }

    int HID_IMPORT_EXPORT Device_Product(int handle, wchar_t* name, int nameSize)
    {
        auto deviceName = LookupDevice(handle).GetProduct();
        return DoStringReturn(name, nameSize, deviceName);
    }

    int HID_IMPORT_EXPORT Device_VID(int handle)
    {
        return LookupDevice(handle).GetVID();
    }

    int HID_IMPORT_EXPORT Device_PID(int handle)
    {
        return LookupDevice(handle).GetPID();
    }



    void HID_IMPORT_EXPORT Collection_Close(int handle)
    {

    }

    int HID_IMPORT_EXPORT Collection_Usage(int handle)
    {
        return 0;
    }

    int HID_IMPORT_EXPORT Collection_CollectionCount(int handle)
    {
        return 0;
    }

    int HID_IMPORT_EXPORT Collection_OpenCollection(int handle, int index)
    {
        return 0;
    }

    int HID_IMPORT_EXPORT Collection_ButtonCount(int handle)
    {
        return 0;
    }

    int HID_IMPORT_EXPORT Collection_ButtonUsage(int handle, int index)
    {
        return 0;
    }

    int HID_IMPORT_EXPORT Collection_ButtonState(int handle, int index)
    {
        // TODO: Implement me
        return 0;
    }

    int HID_IMPORT_EXPORT Collection_ValueCount(int handle)
    {
        return 0;
    }

    int HID_IMPORT_EXPORT Collection_ValueUsage(int handle, int index)
    {
        return 0;
    }

    int HID_IMPORT_EXPORT Collection_ValueValue(int handle, int index)
    {
        // TODO: Implement me
        return 0;
    }

    int HID_IMPORT_EXPORT VID_Name(int vid, wchar_t* name, int nameSize)
    {
        // TODO: Implement me
        return 0;
    }

    int HID_IMPORT_EXPORT PID_Name(int vid, int pid, wchar_t* name, int nameSize)
    {
        // TODO: Implement me
        return 0;
    }

    int HID_IMPORT_EXPORT Usage_Name(int usage)
    {
        // TODO: Implement me
        return 0;
    }
}