#include "pch.h"
#include "DeviceProductProperty.h"

namespace winrt::HIDScout::implementation
{
    Windows::UI::Xaml::Interop::TypeName DeviceProductProperty::Type()
    {
        return { L"String", Windows::UI::Xaml::Interop::TypeKind::Primitive };
    }

    hstring DeviceProductProperty::Name()
    {
        return L"Product";
    }

    Windows::Foundation::IInspectable DeviceProductProperty::GetValue(Windows::Foundation::IInspectable const& target)
    {
        return winrt::box_value(target.as<DeviceItem>().Product());
    }

    void DeviceProductProperty::SetValue(Windows::Foundation::IInspectable const& target, Windows::Foundation::IInspectable const& value)
    {
        DebugBreak(); throw hresult_not_implemented();
    }

    Windows::Foundation::IInspectable DeviceProductProperty::GetIndexedValue(Windows::Foundation::IInspectable const& target, Windows::Foundation::IInspectable const& index)
    {
        DebugBreak(); throw hresult_not_implemented();
    }

    void DeviceProductProperty::SetIndexedValue(Windows::Foundation::IInspectable const& target, Windows::Foundation::IInspectable const& value, Windows::Foundation::IInspectable const& index)
    {
        DebugBreak(); throw hresult_not_implemented();
    }

    bool DeviceProductProperty::CanWrite()
    {
        return false;
    }

    bool DeviceProductProperty::CanRead()
    {
        return true;
    }
}
