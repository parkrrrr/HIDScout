#pragma once

#include "HIDScoutTemplateSelector.g.h"

namespace winrt::HIDScout::implementation
{
    struct HIDScoutTemplateSelector : HIDScoutTemplateSelectorT<HIDScoutTemplateSelector>
    {
        HIDScoutTemplateSelector();

        hstring blah();
        void blah(hstring const& value);

        winrt::Windows::UI::Xaml::DataTemplate SelectTemplateCore(const winrt::Windows::Foundation::IInspectable item);
        winrt::Windows::UI::Xaml::DataTemplate SelectTemplateCore(const winrt::Windows::Foundation::IInspectable item, const winrt::Windows::UI::Xaml::DependencyObject container) {
            return SelectTemplateCore(item);
        }
    private:
        winrt::Windows::UI::Xaml::DataTemplate m_deviceTemplate = nullptr;
        winrt::Windows::UI::Xaml::DataTemplate m_reportTypeTemplate = nullptr;

    };
}

namespace winrt::HIDScout::factory_implementation
{
    struct HIDScoutTemplateSelector : HIDScoutTemplateSelectorT<HIDScoutTemplateSelector, implementation::HIDScoutTemplateSelector>
    {
    };
}
