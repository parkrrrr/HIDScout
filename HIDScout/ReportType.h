#pragma once

#include "ReportType.g.h"

namespace winrt::HIDScout::implementation
{
    struct ReportType : ReportTypeT<ReportType>
    {
        ReportType() = delete;

        hstring TypeName();
        HIDScout::EntryType TIType();
        Windows::UI::Xaml::Data::ICustomProperty GetCustomProperty(hstring const& name);
        Windows::UI::Xaml::Data::ICustomProperty GetIndexedProperty(hstring const& name, Windows::UI::Xaml::Interop::TypeName const& type);
        hstring GetStringRepresentation();
        Windows::UI::Xaml::Interop::TypeName Type();
    };
}
