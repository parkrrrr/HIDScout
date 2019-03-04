#include "pch.h"
#include "Button.h"

namespace winrt::HIDScout::implementation
{
    uint16_t Button::UsagePage()
    {
        DebugBreak(); throw hresult_not_implemented();
    }

    uint16_t Button::Usage()
    {
        DebugBreak(); throw hresult_not_implemented();
    }

    HIDScout::EntryType Button::TIType()
    {
        DebugBreak(); throw hresult_not_implemented();
    }

    Windows::UI::Xaml::Data::ICustomProperty Button::GetCustomProperty(hstring const& name)
    {
        DebugBreak(); throw hresult_not_implemented();
    }

    Windows::UI::Xaml::Data::ICustomProperty Button::GetIndexedProperty(hstring const& name, Windows::UI::Xaml::Interop::TypeName const& type)
    {
        DebugBreak(); throw hresult_not_implemented();
    }

    hstring Button::GetStringRepresentation()
    {
        DebugBreak(); throw hresult_not_implemented();
    }

    Windows::UI::Xaml::Interop::TypeName Button::Type()
    {
        DebugBreak(); throw hresult_not_implemented();
    }
}
