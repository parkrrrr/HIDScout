#include "pch.h"
#include "TreeView.h"
#include "HIDScoutTemplateSelector.h"
#include "DeviceItem.h"

TreeView::TreeView()
{
    ItemTemplateSelector(winrt::make<winrt::HIDScout::implementation::HIDScoutTemplateSelector>());
    FillRootElements();
}

TreeView::~TreeView()
{
    if (devicesHandle)
    {
        //HIDReport::Devices_Close(devicesHandle);
    }
}

void TreeView::FillRootElements()
{
    devicesHandle = HIDReport::Devices_Open();
    if (devicesHandle)
    {
        int count = HIDReport::Devices_Count(devicesHandle);
        for (int i = 0; i < count; ++i)
        {
            int deviceHandle = HIDReport::Devices_OpenDevice(devicesHandle, i);
            if (HIDReport::Device_Opened(deviceHandle))
            {
                winrt::Windows::UI::Xaml::Controls::TreeViewNode node;
                node.Content(winrt::make<winrt::HIDScout::implementation::DeviceItem>(deviceHandle));
                RootNodes().Append(node);
            }
        }
    }
}
