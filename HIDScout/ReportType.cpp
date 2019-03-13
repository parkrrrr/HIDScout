#include "pch.h"
#include "ReportType.h"
#include "TreeViewItemProperty.h"

namespace winrt::HIDScout::implementation
{
    hstring ReportType::TypeName()
    {
        DebugBreak(); throw hresult_not_implemented();
    }

    hstring ReportType::TypeIcon()
    {
        DebugBreak(); throw hresult_not_implemented();
    }

    HIDScout::EntryType ReportType::TIType()
    {
        DebugBreak(); throw hresult_not_implemented();
    }

    Windows::UI::Xaml::Data::ICustomProperty ReportType::GetCustomProperty(hstring const& name)
    {
        return winrt::make<winrt::HIDScout::implementation::TreeViewItemProperty>(HIDScout::EntryType::Device, name).as<Windows::UI::Xaml::Data::ICustomProperty>();
    }

    Windows::UI::Xaml::Data::ICustomProperty ReportType::GetIndexedProperty(hstring const& name, Windows::UI::Xaml::Interop::TypeName const& type)
    {
        DebugBreak(); throw hresult_not_implemented();
    }

    hstring ReportType::GetStringRepresentation()
    {
        DebugBreak(); throw hresult_not_implemented();
    }

    Windows::UI::Xaml::Interop::TypeName ReportType::Type()
    {
        DebugBreak(); throw hresult_not_implemented();
    }
}
