#include "pch.h"
#include "TreeView.h"

TreeView::TreeView() :
	winrt::Windows::UI::Xaml::Controls::TreeView(),
	m_templateSelector()
{
	ItemTemplateSelector(m_templateSelector);
	FillRootElements();
}

void TreeView::FillRootElements()
{
	winrt::Windows::UI::Xaml::Controls::TreeViewNode node;
	node.Content(winrt::box_value(L"Something"));
	RootNodes().Append(node);
}

TreeView::TemplateSelector::TemplateSelector()
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

winrt::Windows::UI::Xaml::DataTemplate TreeView::TemplateSelector::SelectTemplate(winrt::Windows::Foundation::IInspectable item)
{
	return m_deviceTemplate;
}
