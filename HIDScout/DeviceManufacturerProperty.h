#pragma once

#include "DeviceManufacturerProperty.g.h"

namespace winrt::HIDScout::implementation
{
    struct DeviceManufacturerProperty : DeviceManufacturerPropertyT<DeviceManufacturerProperty>
    {
        DeviceManufacturerProperty() = default;

        Windows::UI::Xaml::Interop::TypeName Type();
        hstring Name();
        Windows::Foundation::IInspectable GetValue(Windows::Foundation::IInspectable const& target);
        void SetValue(Windows::Foundation::IInspectable const& target, Windows::Foundation::IInspectable const& value);
        Windows::Foundation::IInspectable GetIndexedValue(Windows::Foundation::IInspectable const& target, Windows::Foundation::IInspectable const& index);
        void SetIndexedValue(Windows::Foundation::IInspectable const& target, Windows::Foundation::IInspectable const& value, Windows::Foundation::IInspectable const& index);
        bool CanWrite();
        bool CanRead();
    };
}

namespace winrt::HIDScout::factory_implementation
{
    struct DeviceManufacturerProperty : DeviceManufacturerPropertyT<DeviceManufacturerProperty, implementation::DeviceManufacturerProperty>
    {
    };
}
