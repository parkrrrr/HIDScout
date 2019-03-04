#include "pch.h"
#include "ReportTypeNameProperty.h"

namespace winrt::HIDScout::implementation
{
    Windows::UI::Xaml::Interop::TypeName ReportTypeNameProperty::Type()
    {
        return { L"String", Windows::UI::Xaml::Interop::TypeKind::Primitive };
    }

    hstring ReportTypeNameProperty::Name()
    {
        return L"TypeName";
    }

    Windows::Foundation::IInspectable ReportTypeNameProperty::GetValue(Windows::Foundation::IInspectable const& target)
    {
        return winrt::box_value(target.as<ReportType>().TypeName());
    }

    void ReportTypeNameProperty::SetValue(Windows::Foundation::IInspectable const& target, Windows::Foundation::IInspectable const& value)
    {
        DebugBreak(); throw hresult_not_implemented();
    }

    Windows::Foundation::IInspectable ReportTypeNameProperty::GetIndexedValue(Windows::Foundation::IInspectable const& target, Windows::Foundation::IInspectable const& index)
    {
        DebugBreak(); throw hresult_not_implemented();
    }

    void ReportTypeNameProperty::SetIndexedValue(Windows::Foundation::IInspectable const& target, Windows::Foundation::IInspectable const& value, Windows::Foundation::IInspectable const& index)
    {
        DebugBreak(); throw hresult_not_implemented();
    }

    bool ReportTypeNameProperty::CanWrite()
    {
        return false;
    }

    bool ReportTypeNameProperty::CanRead()
    {
        return true;
    }
}
