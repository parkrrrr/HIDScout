#include "pch.h"
#include "GenericUsageProperty.h"

namespace winrt::HIDScout::implementation
{
    Windows::UI::Xaml::Interop::TypeName GenericUsageProperty::Type()
    {
        return { L"UInt16", Windows::UI::Xaml::Interop::TypeKind::Primitive };
    }

    hstring GenericUsageProperty::Name()
    {
        return L"Usage";
    }

    Windows::Foundation::IInspectable GenericUsageProperty::GetValue(Windows::Foundation::IInspectable const& target)
    {
        auto itemType = target.as<HIDScout::ITreeItemType>().TIType();
        switch (itemType)
        {
        case HIDScout::EntryType::Button:
            return winrt::box_value(target.as<HIDScout::Button>().Usage());

        case HIDScout::EntryType::Value:
            return winrt::box_value(target.as<HIDScout::Value>().Usage());

        case HIDScout::EntryType::Collection:
            return winrt::box_value(target.as<HIDScout::Collection>().Usage());

        case HIDScout::EntryType::Report:
            return winrt::box_value(target.as<HIDScout::Report>().Usage());
        }
        DebugBreak(); throw hresult_not_implemented();
    }

    void GenericUsageProperty::SetValue(Windows::Foundation::IInspectable const& target, Windows::Foundation::IInspectable const& value)
    {
        DebugBreak(); throw hresult_not_implemented();
    }

    Windows::Foundation::IInspectable GenericUsageProperty::GetIndexedValue(Windows::Foundation::IInspectable const& target, Windows::Foundation::IInspectable const& index)
    {
        DebugBreak(); throw hresult_not_implemented();
    }

    void GenericUsageProperty::SetIndexedValue(Windows::Foundation::IInspectable const& target, Windows::Foundation::IInspectable const& value, Windows::Foundation::IInspectable const& index)
    {
        DebugBreak(); throw hresult_not_implemented();
    }

    bool GenericUsageProperty::CanWrite()
    {
        return false;
    }

    bool GenericUsageProperty::CanRead()
    {
        return true;
    }
}
