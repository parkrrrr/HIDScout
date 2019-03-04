#include "pch.h"
#include "DeviceVIDProperty.h"

namespace winrt::HIDScout::implementation
{
    Windows::UI::Xaml::Interop::TypeName DeviceVIDProperty::Type()
    {
        return { L"String", Windows::UI::Xaml::Interop::TypeKind::Primitive };
    }

    hstring DeviceVIDProperty::Name()
    {
        return L"VID";
    }

    Windows::Foundation::IInspectable DeviceVIDProperty::GetValue(Windows::Foundation::IInspectable const& target)
    {
        auto vid = target.as<DeviceItem>().VID();
        std::wstringstream stream;
        stream << L" " << std::hex << std::setw(4) << std::setfill(L'0') << vid;
        return winrt::box_value(hstring(stream.str()));
    }

    void DeviceVIDProperty::SetValue(Windows::Foundation::IInspectable const& target, Windows::Foundation::IInspectable const& value)
    {
        DebugBreak(); throw hresult_not_implemented();
    }

    Windows::Foundation::IInspectable DeviceVIDProperty::GetIndexedValue(Windows::Foundation::IInspectable const& target, Windows::Foundation::IInspectable const& index)
    {
        DebugBreak(); throw hresult_not_implemented();
    }

    void DeviceVIDProperty::SetIndexedValue(Windows::Foundation::IInspectable const& target, Windows::Foundation::IInspectable const& value, Windows::Foundation::IInspectable const& index)
    {
        DebugBreak(); throw hresult_not_implemented();
    }

    bool DeviceVIDProperty::CanWrite()
    {
        return false;
    }

    bool DeviceVIDProperty::CanRead()
    {
        return true;
    }
}
