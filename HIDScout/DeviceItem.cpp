#include "pch.h"
#include "DeviceItem.h"
#include "DeviceVIDProperty.h"
#include "DevicePIDProperty.h"
#include "DeviceManufacturerProperty.h"
#include "DeviceProductProperty.h"

namespace winrt::HIDScout::implementation
{
    DeviceItem::DeviceItem(int deviceHandle) :
        m_deviceHandle(deviceHandle)
    {
        m_vid = HIDReport::Device_VID(m_deviceHandle);
        m_pid = HIDReport::Device_PID(m_deviceHandle);

        const size_t USB_STRING_SIZE = 127; // 126 is max USB size, +1 for null terminator
        
        wchar_t tempString[USB_STRING_SIZE] = {};
        HIDReport::Device_Manufacturer(m_deviceHandle, tempString, USB_STRING_SIZE);
        m_manufacturer = tempString;
        
        tempString[0] = 0;
        HIDReport::Device_Product(m_deviceHandle, tempString, USB_STRING_SIZE);
        m_product = tempString;
    }

    DeviceItem::~DeviceItem()
    {
        HIDReport::Device_Close(m_deviceHandle);
    }

    uint16_t DeviceItem::VID()
    {
        return m_vid;
    }

    uint16_t DeviceItem::PID()
    {
        return m_pid;
    }

    hstring DeviceItem::Manufacturer()
    {
        return hstring(m_manufacturer);
    }

    hstring DeviceItem::Product()
    {
        return hstring(m_product);
    }

    HIDScout::EntryType DeviceItem::TIType()
    {
        return HIDScout::EntryType::Device;
    }

    Windows::UI::Xaml::Data::ICustomProperty DeviceItem::GetCustomProperty(hstring const& name)
    {
        std::wstring sname(name);

        if (sname == L"VID") return winrt::make<winrt::HIDScout::implementation::DeviceVIDProperty>().as<Windows::UI::Xaml::Data::ICustomProperty>();
        if (sname == L"PID") return winrt::make<winrt::HIDScout::implementation::DevicePIDProperty>().as<Windows::UI::Xaml::Data::ICustomProperty>();
        if (sname == L"Manufacturer") return winrt::make<winrt::HIDScout::implementation::DeviceManufacturerProperty>().as<Windows::UI::Xaml::Data::ICustomProperty>();
        if (sname == L"Product") return winrt::make<winrt::HIDScout::implementation::DeviceProductProperty>().as<Windows::UI::Xaml::Data::ICustomProperty>();
        DebugBreak(); throw hresult_not_implemented();
    }

    Windows::UI::Xaml::Data::ICustomProperty DeviceItem::GetIndexedProperty(hstring const& name, Windows::UI::Xaml::Interop::TypeName const& type)
    {
        DebugBreak(); throw hresult_not_implemented();
    }

    hstring DeviceItem::GetStringRepresentation()
    {
        DebugBreak(); throw hresult_not_implemented();
    }

    Windows::UI::Xaml::Interop::TypeName DeviceItem::Type()
    {
        return { L"HIDScout.DeviceItem", Windows::UI::Xaml::Interop::TypeKind::Primitive };
    }
}
