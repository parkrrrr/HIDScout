#include "pch.h"
#include "Report.h"

namespace winrt::HIDScout::implementation
{
    uint16_t Report::UsagePage()
    {
        DebugBreak(); throw hresult_not_implemented();
    }

    uint16_t Report::Usage()
    {
        DebugBreak(); throw hresult_not_implemented();
    }

    HIDScout::EntryType Report::TIType()
    {
        DebugBreak(); throw hresult_not_implemented();
    }

    Windows::UI::Xaml::Data::ICustomProperty Report::GetCustomProperty(hstring const& name)
    {
        DebugBreak(); throw hresult_not_implemented();
    }

    Windows::UI::Xaml::Data::ICustomProperty Report::GetIndexedProperty(hstring const& name, Windows::UI::Xaml::Interop::TypeName const& type)
    {
        DebugBreak(); throw hresult_not_implemented();
    }

    hstring Report::GetStringRepresentation()
    {
        DebugBreak(); throw hresult_not_implemented();
    }

    Windows::UI::Xaml::Interop::TypeName Report::Type()
    {
        DebugBreak(); throw hresult_not_implemented();
    }
}
