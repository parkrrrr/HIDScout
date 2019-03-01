#include "pch.h"
#include "HIDScoutTemplateSelector.h"

namespace winrt::HIDScout::implementation
{
    HIDScoutTemplateSelector::HIDScoutTemplateSelector()
    {
        std::wstring deviceXaml = LR"**(
		<DataTemplate xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation">
			<TreeViewItem>
				<TextBlock Text="{Binding Content}" />
			</TreeViewItem>
		</DataTemplate>
	)**";

        m_deviceTemplate = winrt::Windows::UI::Xaml::Markup::XamlReader(nullptr).Load(deviceXaml)
            .as<winrt::Windows::UI::Xaml::DataTemplate>();
    }

    winrt::Windows::UI::Xaml::DataTemplate HIDScoutTemplateSelector::SelectTemplateCore(winrt::Windows::Foundation::IInspectable item)
    {
        return m_deviceTemplate;
    }


    hstring HIDScoutTemplateSelector::blah()
    {
        throw hresult_not_implemented();
    }

    void HIDScoutTemplateSelector::blah(hstring const& value)
    {
        throw hresult_not_implemented();
    }
}
