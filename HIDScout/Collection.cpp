#include "pch.h"
#include "Collection.h"

namespace winrt::HIDScout::implementation
{
    uint16_t Collection::UsagePage()
    {
        DebugBreak(); throw hresult_not_implemented();
    }

    uint16_t Collection::Usage()
    {
        DebugBreak(); throw hresult_not_implemented();
    }

    HIDScout::EntryType Collection::TIType()
    {
        DebugBreak(); throw hresult_not_implemented();
    }

    Windows::UI::Xaml::Data::ICustomProperty Collection::GetCustomProperty(hstring const& name)
    {
        DebugBreak(); throw hresult_not_implemented();
    }

    Windows::UI::Xaml::Data::ICustomProperty Collection::GetIndexedProperty(hstring const& name, Windows::UI::Xaml::Interop::TypeName const& type)
    {
        DebugBreak(); throw hresult_not_implemented();
    }

    hstring Collection::GetStringRepresentation()
    {
        DebugBreak(); throw hresult_not_implemented();
    }

    Windows::UI::Xaml::Interop::TypeName Collection::Type()
    {
        DebugBreak(); throw hresult_not_implemented();
    }
}
