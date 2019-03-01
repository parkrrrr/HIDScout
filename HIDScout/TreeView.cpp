#include "pch.h"
#include "TreeView.h"
#include "HIDScoutTemplateSelector.h"

TreeView::TreeView()
{
    ItemTemplateSelector(winrt::make<winrt::HIDScout::implementation::HIDScoutTemplateSelector>());
    FillRootElements();
}

void TreeView::FillRootElements()
{
    winrt::Windows::UI::Xaml::Controls::TreeViewNode node;
    node.Content(winrt::box_value(L"Something"));
    RootNodes().Append(node);

    winrt::Windows::UI::Xaml::Controls::TreeViewNode node2;
    node2.Content(winrt::box_value(L"Something Else"));
    node.Children().Append(node2);
}
