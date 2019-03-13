#include "pch.h"
#include "TreeViewItemProperty.h"

namespace winrt::HIDScout::implementation
{
    Windows::UI::Xaml::Interop::TypeName TreeViewItemProperty::Type()
    {
        return { L"String", Windows::UI::Xaml::Interop::TypeKind::Primitive };
    }

    hstring TreeViewItemProperty::Name()
    {
        return hstring(m_propertyName);
    }

    Windows::Foundation::IInspectable TreeViewItemProperty::GetValue(Windows::Foundation::IInspectable const& target)
    {
        winrt::hstring resultString;
        switch (m_entryType)
        {
        case HIDScout::EntryType::Device:
            if (m_propertyName == L"VID" || m_propertyName == L"PID")
            {
                uint16_t id = 0;
                if (m_propertyName == L"VID")
                {
                    id = target.as<DeviceItem>().PID();
                }
                else
                {
                    id = target.as<DeviceItem>().VID();
                }
                std::wstringstream stream;
                stream << std::hex << std::setw(4) << std::setfill(L'0') << id;
                resultString = stream.str();
            }
            else if (m_propertyName == L"Manufacturer")
            {
                resultString = target.as<DeviceItem>().Manufacturer();
            }
            else if (m_propertyName == L"Product")
            {
                resultString = target.as<DeviceItem>().Product();
            }
            break;

        case HIDScout::EntryType::ReportType:
            if (m_propertyName == L"TypeIcon")
            {
                resultString = target.as<ReportType>().TypeIcon();
            }
            else
            {
                resultString = target.as<ReportType>().TypeName();
            }
            break;

        default:
            break;
        }
        return winrt::box_value(hstring(resultString));
    }

    void TreeViewItemProperty::SetValue(Windows::Foundation::IInspectable const& target, Windows::Foundation::IInspectable const& value)
    {
        throw hresult_not_implemented();
    }

    Windows::Foundation::IInspectable TreeViewItemProperty::GetIndexedValue(Windows::Foundation::IInspectable const& target, Windows::Foundation::IInspectable const& index)
    {
        throw hresult_not_implemented();
    }

    void TreeViewItemProperty::SetIndexedValue(Windows::Foundation::IInspectable const& target, Windows::Foundation::IInspectable const& value, Windows::Foundation::IInspectable const& index)
    {
        throw hresult_not_implemented();
    }

    bool TreeViewItemProperty::CanWrite()
    {
        return false;
    }

    bool TreeViewItemProperty::CanRead()
    {
        return true;
    }
}
