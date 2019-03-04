#include "pch.h"
#include "Value.h"

namespace winrt::HIDScout::implementation
{
    uint16_t Value::UsagePage()
    {
        DebugBreak(); throw hresult_not_implemented();
    }

    uint16_t Value::Usage()
    {
        DebugBreak(); throw hresult_not_implemented();
    }

    HIDScout::EntryType Value::TIType()
    {
        DebugBreak(); throw hresult_not_implemented();
    }

    Windows::UI::Xaml::Data::ICustomProperty Value::GetCustomProperty(hstring const& name)
    {
        DebugBreak(); throw hresult_not_implemented();
    }

    Windows::UI::Xaml::Data::ICustomProperty Value::GetIndexedProperty(hstring const& name, Windows::UI::Xaml::Interop::TypeName const& type)
    {
        DebugBreak(); throw hresult_not_implemented();
    }

    hstring Value::GetStringRepresentation()
    {
        DebugBreak(); throw hresult_not_implemented();
    }

    Windows::UI::Xaml::Interop::TypeName Value::Type()
    {
        DebugBreak(); throw hresult_not_implemented();
    }
}
