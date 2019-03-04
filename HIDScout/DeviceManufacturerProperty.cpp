#include "pch.h"
#include "DeviceManufacturerProperty.h"

namespace winrt::HIDScout::implementation
{
    Windows::UI::Xaml::Interop::TypeName DeviceManufacturerProperty::Type()
    {
        return { L"String", Windows::UI::Xaml::Interop::TypeKind::Primitive };
    }

    hstring DeviceManufacturerProperty::Name()
    {
        return L"Manufacturer";
    }

    Windows::Foundation::IInspectable DeviceManufacturerProperty::GetValue(Windows::Foundation::IInspectable const& target)
    {
        return winrt::box_value(target.as<DeviceItem>().Manufacturer());
    }

    void DeviceManufacturerProperty::SetValue(Windows::Foundation::IInspectable const& target, Windows::Foundation::IInspectable const& value)
    {
        DebugBreak(); throw hresult_not_implemented();
    }

    Windows::Foundation::IInspectable DeviceManufacturerProperty::GetIndexedValue(Windows::Foundation::IInspectable const& target, Windows::Foundation::IInspectable const& index)
    {
        DebugBreak(); throw hresult_not_implemented();
    }

    void DeviceManufacturerProperty::SetIndexedValue(Windows::Foundation::IInspectable const& target, Windows::Foundation::IInspectable const& value, Windows::Foundation::IInspectable const& index)
    {
        DebugBreak(); throw hresult_not_implemented();
    }

    bool DeviceManufacturerProperty::CanWrite()
    {
        return false;
    }

    bool DeviceManufacturerProperty::CanRead()
    {
        return true;
    }
}
