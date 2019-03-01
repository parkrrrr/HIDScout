#pragma once
#include "HIDScoutTemplateSelector.h"

class TreeView : public winrt::Windows::UI::Xaml::Controls::TreeView
{
public:
    TreeView();

private:
    void FillRootElements();
};