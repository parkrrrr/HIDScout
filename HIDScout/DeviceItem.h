#pragma once

#include "DeviceItem.g.h"

namespace winrt::HIDScout::implementation
{
    struct DeviceItem : DeviceItemT<DeviceItem>
    {
        DeviceItem() = delete;
        DeviceItem(int deviceHandle);

        ~DeviceItem();

        uint16_t VID();
        uint16_t PID();
        hstring Manufacturer();
        hstring Product();
        HIDScout::EntryType TIType();
        Windows::UI::Xaml::Data::ICustomProperty GetCustomProperty(hstring const& name);
        Windows::UI::Xaml::Data::ICustomProperty GetIndexedProperty(hstring const& name, Windows::UI::Xaml::Interop::TypeName const& type);
        hstring GetStringRepresentation();
        Windows::UI::Xaml::Interop::TypeName Type();       

    private:
        uint16_t m_vid = 0;
        uint16_t m_pid = 0;
        std::wstring m_manufacturer;
        std::wstring m_product;

        int m_deviceHandle = 0;
    };
}
