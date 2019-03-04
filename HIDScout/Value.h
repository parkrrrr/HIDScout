#pragma once

#include "Value.g.h"

namespace winrt::HIDScout::implementation
{
    struct Value : ValueT<Value>
    {
        Value() = delete;

        uint16_t UsagePage();
        uint16_t Usage();
        HIDScout::EntryType TIType();
        Windows::UI::Xaml::Data::ICustomProperty GetCustomProperty(hstring const& name);
        Windows::UI::Xaml::Data::ICustomProperty GetIndexedProperty(hstring const& name, Windows::UI::Xaml::Interop::TypeName const& type);
        hstring GetStringRepresentation();
        Windows::UI::Xaml::Interop::TypeName Type();
    };
}
