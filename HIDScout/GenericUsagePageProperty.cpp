#include "pch.h"
#include "GenericUsagePageProperty.h"

namespace winrt::HIDScout::implementation
{
    Windows::UI::Xaml::Interop::TypeName GenericUsagePageProperty::Type()
    {
        return { L"UInt16", Windows::UI::Xaml::Interop::TypeKind::Primitive };
    }

    hstring GenericUsagePageProperty::Name()
    {
        return L"Usagepage";
    }

    Windows::Foundation::IInspectable GenericUsagePageProperty::GetValue(Windows::Foundation::IInspectable const& target)
    {
        auto itemType = target.as<HIDScout::ITreeItemType>().TIType();
        switch (itemType)
        {
        case HIDScout::EntryType::Button:
            return winrt::box_value(target.as<HIDScout::Button>().UsagePage());

        case HIDScout::EntryType::Value:
            return winrt::box_value(target.as<HIDScout::Value>().UsagePage());

        case HIDScout::EntryType::Collection:
            return winrt::box_value(target.as<HIDScout::Collection>().UsagePage());

        case HIDScout::EntryType::Report:
            return winrt::box_value(target.as<HIDScout::Report>().UsagePage());
        }
        DebugBreak(); throw hresult_not_implemented();
    }

    void GenericUsagePageProperty::SetValue(Windows::Foundation::IInspectable const& target, Windows::Foundation::IInspectable const& value)
    {
        DebugBreak(); throw hresult_not_implemented();
    }

    Windows::Foundation::IInspectable GenericUsagePageProperty::GetIndexedValue(Windows::Foundation::IInspectable const& target, Windows::Foundation::IInspectable const& index)
    {
        DebugBreak(); throw hresult_not_implemented();
    }

    void GenericUsagePageProperty::SetIndexedValue(Windows::Foundation::IInspectable const& target, Windows::Foundation::IInspectable const& value, Windows::Foundation::IInspectable const& index)
    {
        DebugBreak(); throw hresult_not_implemented();
    }

    bool GenericUsagePageProperty::CanWrite()
    {
        return false;
    }

    bool GenericUsagePageProperty::CanRead()
    {
        return true;
    }
}
