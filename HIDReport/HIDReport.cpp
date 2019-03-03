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

    class Device;

    class Collection
    {
    public:
        Collection(Device& device, int type, int id, int collectionIndex) :
            m_device(device), m_type(type), m_id(id), m_collectionIndex(collectionIndex)
        {

        }

        int Usage();
        int CollectionCount();
        int OpenCollection(int index);
        int ButtonCount();
        int ButtonUsage(int index);
        int ValueCount();
        int ValueUsage(int index);

    private:
        Device& m_device;
        int m_type;
        int m_id;
        int m_collectionIndex;
    };

    std::map<int, Collection> collections;

    Collection& LookupCollection(int handle)
    {
        auto iter = collections.find(handle);
        if (iter != collections.end())
        {
            return iter->second;
        }
        throw(std::runtime_error("Collection Not Found"));
    }

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

        int OpenReportCollection(int type, int id)
        {
            return OpenCollection(type, id, -1, 0);
        }

        int GetCollectionUsage(int collectionIndex)
        {
            return (m_collectionNodes[collectionIndex].LinkUsagePage << 16) | m_collectionNodes[collectionIndex].LinkUsage;
        }

        int GetCollectionCount(int type, int id, int collectionIndex)
        {
            int count = 0;
            int childIndex = m_collectionNodes[collectionIndex].FirstChild;
            while (childIndex)
            {
                if ((m_collectionTypes[childIndex] & (1 << type)) && (NodeHasId(childIndex, id)))
                {
                    ++count;
                }
                childIndex = m_collectionNodes[childIndex].NextSibling;
            }
            return count;
        }

        int OpenCollection(int type, int id, int parentCollectionIndex, int childIndexWithinCollection)
        {
            if (parentCollectionIndex == -1)
            {
                return CreateCollection(type, id, 0);
            }

            int count = 0;
            int childIndex = m_collectionNodes[parentCollectionIndex].FirstChild;
            while (childIndex)
            {
                if ((m_collectionTypes[childIndex] & (1 << type)) && (NodeHasId(childIndex, id)))
                {
                    if (count == childIndexWithinCollection)
                    {
                        return CreateCollection(type, id, childIndex);
                    }
                    ++count;
                }
                childIndex = m_collectionNodes[childIndex].NextSibling;
            }
            return 0;
        }

        int GetButtonCount(int type, int id, int collectionIndex)
        {
            int count = 0;
            for (int i = 0; i < m_buttonCount[type]; ++i)
            {
                auto& buttonCap = m_buttonCaps[type][i];
                if ((buttonCap.ReportID == id) && (buttonCap.LinkCollection == collectionIndex) && (!buttonCap.IsAlias))
                {
                    if (buttonCap.IsRange)
                    {
                        count += buttonCap.Range.UsageMax - buttonCap.Range.UsageMin + 1;
                    }
                    else
                    {
                        count += 1;
                    }
                }
            }
            return count;
        }

        int GetButtonUsage(int type, int id, int collectionIndex, int buttonIndex)
        {
            int count = 0;
            for (int i = 0; i < m_buttonCount[type]; ++i)
            {
                auto& buttonCap = m_buttonCaps[type][i];
                if ((buttonCap.ReportID == id) && (buttonCap.LinkCollection == collectionIndex) && (!buttonCap.IsAlias))
                {
                    if (buttonCap.IsRange)
                    {
                        auto rangeCount = buttonCap.Range.UsageMax - buttonCap.Range.UsageMin + 1;

                        if (count + rangeCount > buttonIndex)
                        {
                            return (buttonCap.UsagePage << 16) | (buttonCap.Range.UsageMin + buttonIndex - count);
                        }

                        count += rangeCount;
                    }
                    else
                    {
                        if (count == buttonIndex)
                        {
                            return (buttonCap.UsagePage << 16) | buttonCap.NotRange.Usage;
                        }
                        count += 1;
                    }
                }
            }
            return 0;
        }

        int GetValueCount(int type, int id, int collectionIndex)
        {
            int count = 0;
            for (int i = 0; i < m_valueCount[type]; ++i)
            {
                auto& valueCap = m_valueCaps[type][i];
                if ((valueCap.ReportID == id) && (valueCap.LinkCollection == collectionIndex) && (!valueCap.IsAlias))
                {
                    count += valueCap.ReportCount;
                }
            }
            return count;
        }

        int GetValueUsage(int type, int id, int collectionIndex, int valueIndex)
        {
            int count = 0;
            for (int i = 0; i < m_valueCount[type]; ++i)
            {
                auto& valueCap = m_valueCaps[type][i];
                if ((valueCap.ReportID == id) && (valueCap.LinkCollection == collectionIndex) && (!valueCap.IsAlias))
                {
                    if (count + valueCap.ReportCount > valueIndex)
                    {
                        if (valueCap.IsRange)
                        {
                            int usage = valueCap.Range.UsageMin + valueIndex - count;
                            if (usage > valueCap.Range.UsageMax) {
                                usage = valueCap.Range.UsageMax;
                                return (valueCap.UsagePage << 16) | usage;
                            }
                        }
                        else {
                            return (valueCap.UsagePage << 16) | valueCap.NotRange.Usage;
                        }
                    }
                    count += valueCap.ReportCount;
                }
            }
            return count;
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
            
            // Get all of the collections and initialize their types to 'unused' and their IDs to empty
            unsigned long length = m_topLevelCaps.NumberLinkCollectionNodes;
            m_collectionNodes = std::make_unique<HIDP_LINK_COLLECTION_NODE[]>(length);
            HidP_GetLinkCollectionNodes(m_collectionNodes.get(), &length, m_preparsedData.Get());

            m_collectionTypes.insert(m_collectionTypes.cbegin(), length, 0);

            for (unsigned long i = 0; i < length; ++i)
            {
                m_collectionIDs.push_back(std::vector<int>());
            }

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

        bool NodeHasId(int collectionID, int id)
        {
            return (std::find(m_collectionIDs[collectionID].begin(), m_collectionIDs[collectionID].end(), id) != m_collectionIDs[collectionID].end());
        }

        void UpdateLinkCollectionTypesAndIDs(unsigned short collectionID, HIDP_REPORT_TYPE type, int id)
        {
            int flag = 1 << type;

            do
            {
                m_collectionTypes[collectionID] |= flag;
                if (!NodeHasId(collectionID, id))
                {
                    m_collectionIDs[collectionID].push_back(id);
                }
                collectionID = m_collectionNodes[collectionID].Parent;
            } while (collectionID);
        }

        void GetButtonCaps(HIDP_REPORT_TYPE type, unsigned short count)
        {
            m_buttonCount[type] = count;
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
                UpdateLinkCollectionTypesAndIDs(buttonCap.LinkCollection, type, buttonCap.ReportID);
            }
        }

        void GetValueCaps(HIDP_REPORT_TYPE type, unsigned short count)
        {
            m_valueCount[type] = count;
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
                UpdateLinkCollectionTypesAndIDs(valueCap.LinkCollection, type, valueCap.ReportID);
            }
        }

        int CreateCollection(int type, int id, int collectionIndex)
        {
            int handle = HandleFactory::GetNextHandle();
            collections.emplace(std::make_pair(handle, Collection(*this, type, id, collectionIndex)));
            return handle;
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
        
        int m_buttonCount[3] = {};
        std::unique_ptr<HIDP_BUTTON_CAPS[]> m_buttonCaps[3];

        int m_valueCount[3] = {};
        std::unique_ptr<HIDP_VALUE_CAPS[]> m_valueCaps[3];

        const int COL_INPUT = 1 << HidP_Input;
        const int COL_OUTPUT = 1 << HidP_Output;
        const int COL_FEATURE = 1 << HidP_Feature;
        std::vector<int> m_collectionTypes;
        std::vector<std::vector<int>> m_collectionIDs;

    };

    int Collection::Usage()
    {
        return m_device.GetCollectionUsage(m_collectionIndex);
    }

    int Collection::CollectionCount()
    {
        return m_device.GetCollectionCount(m_type, m_id, m_collectionIndex);
    }

    int Collection::OpenCollection(int index)
    {
        return m_device.OpenCollection(m_type, m_id, m_collectionIndex, index);
    }

    int Collection::ButtonCount()
    {
        return m_device.GetButtonCount(m_type, m_id, m_collectionIndex);
    }

    int Collection::ButtonUsage(int index)
    {
        return m_device.GetButtonUsage(m_type, m_id, m_collectionIndex, index);
    }

    int Collection::ValueCount()
    {
        return m_device.GetValueCount(m_type, m_id, m_collectionIndex);
    }

    int Collection::ValueUsage(int index)
    {
        return m_device.GetValueUsage(m_type, m_id, m_collectionIndex, index);
    }



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
        return LookupDevice(handle).OpenReportCollection(type, id);
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
        collections.erase(handle);
    }

    int HID_IMPORT_EXPORT Collection_Usage(int handle)
    {
        return LookupCollection(handle).Usage();
    }

    int HID_IMPORT_EXPORT Collection_CollectionCount(int handle)
    {
        return LookupCollection(handle).CollectionCount();
    }

    int HID_IMPORT_EXPORT Collection_OpenCollection(int handle, int index)
    {
        return LookupCollection(handle).OpenCollection(index);
    }

    int HID_IMPORT_EXPORT Collection_ButtonCount(int handle)
    {
        return LookupCollection(handle).ButtonCount();
    }

    int HID_IMPORT_EXPORT Collection_ButtonUsage(int handle, int index)
    {
        return LookupCollection(handle).ButtonUsage(index);
    }

    int HID_IMPORT_EXPORT Collection_ButtonState(int handle, int index)
    {
        // TODO: Implement me
        return 0;
    }

    int HID_IMPORT_EXPORT Collection_ValueCount(int handle)
    {
        return LookupCollection(handle).ValueCount();
    }

    int HID_IMPORT_EXPORT Collection_ValueUsage(int handle, int index)
    {
        return LookupCollection(handle).ValueUsage(index);
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