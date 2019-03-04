#include "pch.h"
#include "DevicePIDProperty.h"

namespace winrt::HIDScout::implementation
{
    Windows::UI::Xaml::Interop::TypeName DevicePIDProperty::Type()
    {
        return { L"String", Windows::UI::Xaml::Interop::TypeKind::Primitive };
    }

    hstring DevicePIDProperty::Name()
    {
        return L"PID";
    }

    Windows::Foundation::IInspectable DevicePIDProperty::GetValue(Windows::Foundation::IInspectable const& target)
    {
        auto pid = target.as<DeviceItem>().PID();
        std::wstringstream stream;
        stream << L" " << std::hex << std::setw(4) << std::setfill(L'0') << pid;
        return winrt::box_value(hstring(stream.str()));
    }

    void DevicePIDProperty::SetValue(Windows::Foundation::IInspectable const& target, Windows::Foundation::IInspectable const& value)
    {
        DebugBreak(); throw hresult_not_implemented();
    }

    Windows::Foundation::IInspectable DevicePIDProperty::GetIndexedValue(Windows::Foundation::IInspectable const& target, Windows::Foundation::IInspectable const& index)
    {
        DebugBreak(); throw hresult_not_implemented();
    }

    void DevicePIDProperty::SetIndexedValue(Windows::Foundation::IInspectable const& target, Windows::Foundation::IInspectable const& value, Windows::Foundation::IInspectable const& index)
    {
        DebugBreak(); throw hresult_not_implemented();
    }

    bool DevicePIDProperty::CanWrite()
    {
        return false;
    }

    bool DevicePIDProperty::CanRead()
    {
        return true;
    }
}
