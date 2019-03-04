#include "pch.h"
#include "HIDScoutTemplateSelector.h"

namespace winrt::HIDScout::implementation
{
    HIDScoutTemplateSelector::HIDScoutTemplateSelector()
    {
        std::wstring deviceXaml = LR"**(
		    <DataTemplate xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation">
			    <TreeViewItem Padding="3">
                    <StackPanel>
                        <StackPanel Orientation="Horizontal">
                            <TextBlock Text="Vendor ID" FontWeight="Bold" Padding="30,0,10,0" />
                            <TextBlock Text="{Binding Content.VID}" />
                            <TextBlock Text="Product ID" FontWeight="Bold" Padding="30,0,10,0"/>
                            <TextBlock Text="{Binding Content.PID}" />
                        </StackPanel>
                        <TextBlock Text="{Binding Content.Manufacturer}" />
                        <TextBlock Text="{Binding Content.Product}" />
                    </StackPanel>
			    </TreeViewItem>
		    </DataTemplate>
	    )**";

        m_deviceTemplate = winrt::Windows::UI::Xaml::Markup::XamlReader(nullptr).Load(deviceXaml)
            .as<winrt::Windows::UI::Xaml::DataTemplate>();

        std::wstring reportTypeXaml = LR"**(
		    <DataTemplate xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation">
			    <TreeViewItem>
                    <TextBlock Text="{Binding Content.ReportType}" />
			    </TreeViewItem>
		    </DataTemplate>
	    )**";

        m_reportTypeTemplate = winrt::Windows::UI::Xaml::Markup::XamlReader(nullptr).Load(reportTypeXaml)
            .as<winrt::Windows::UI::Xaml::DataTemplate>();
    }

    winrt::Windows::UI::Xaml::DataTemplate HIDScoutTemplateSelector::SelectTemplateCore(winrt::Windows::Foundation::IInspectable item)
    {
        auto treeNode = item.as<winrt::Windows::UI::Xaml::Controls::TreeViewNode>();

        auto itemType = treeNode.Content().as<HIDScout::ITreeItemType>().TIType();
        
        switch (itemType)
        {
        case HIDScout::EntryType::Device:
            return m_deviceTemplate;

        case HIDScout::EntryType::ReportType:
            return m_reportTypeTemplate;
        }

        return m_deviceTemplate;
    }

    hstring HIDScoutTemplateSelector::blah()
    {
        DebugBreak(); throw hresult_not_implemented();
    }

    void HIDScoutTemplateSelector::blah(hstring const& value)
    {
        DebugBreak(); throw hresult_not_implemented();
    }
}
