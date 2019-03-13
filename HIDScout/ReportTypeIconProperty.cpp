#include "pch.h"
#include "ReportTypeIconProperty.h"

namespace winrt::HIDScout::implementation
{
    Windows::UI::Xaml::Interop::TypeName ReportTypeIconProperty::Type()
    {
        return { L"String", Windows::UI::Xaml::Interop::TypeKind::Primitive };
    }

    hstring ReportTypeIconProperty::Name()
    {
        return L"TypeIcon";
    }

    Windows::Foundation::IInspectable ReportTypeIconProperty::GetValue(Windows::Foundation::IInspectable const& target)
    {
        return winrt::box_value(target.as<ReportType>().TypeIcon());
    }

    void ReportTypeIconProperty::SetValue(Windows::Foundation::IInspectable const& target, Windows::Foundation::IInspectable const& value)
    {
        throw hresult_not_implemented();
    }

    Windows::Foundation::IInspectable ReportTypeIconProperty::GetIndexedValue(Windows::Foundation::IInspectable const& target, Windows::Foundation::IInspectable const& index)
    {
        throw hresult_not_implemented();
    }

    void ReportTypeIconProperty::SetIndexedValue(Windows::Foundation::IInspectable const& target, Windows::Foundation::IInspectable const& value, Windows::Foundation::IInspectable const& index)
    {
        throw hresult_not_implemented();
    }

    bool ReportTypeIconProperty::CanWrite()
    {
        return false;
    }

    bool ReportTypeIconProperty::CanRead()
    {
        return true;
    }
}
