#pragma once

#include "TreeViewItemProperty.g.h"

namespace winrt::HIDScout::implementation
{
    struct TreeViewItemProperty : TreeViewItemPropertyT<TreeViewItemProperty>
    {
        TreeViewItemProperty() = default;

        TreeViewItemProperty(HIDScout::EntryType entryType, hstring propertyName) : 
            m_entryType(entryType),
            m_propertyName(propertyName)
        {}

        Windows::UI::Xaml::Interop::TypeName Type();
        hstring Name();
        Windows::Foundation::IInspectable GetValue(Windows::Foundation::IInspectable const& target);
        void SetValue(Windows::Foundation::IInspectable const& target, Windows::Foundation::IInspectable const& value);
        Windows::Foundation::IInspectable GetIndexedValue(Windows::Foundation::IInspectable const& target, Windows::Foundation::IInspectable const& index);
        void SetIndexedValue(Windows::Foundation::IInspectable const& target, Windows::Foundation::IInspectable const& value, Windows::Foundation::IInspectable const& index);
        bool CanWrite();
        bool CanRead();
    private:
        HIDScout::EntryType m_entryType = HIDScout::EntryType::Device;
        std::wstring m_propertyName;
    };
}

namespace winrt::HIDScout::factory_implementation
{
    struct TreeViewItemProperty : TreeViewItemPropertyT<TreeViewItemProperty, implementation::TreeViewItemProperty>
    {
    };
}
