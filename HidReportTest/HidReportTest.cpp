// HidReportTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

namespace HID
{
#include "..\HIDReport\HIDReport.h"
}

std::string Indent(size_t level)
{
    return std::string(level * 3, ' ');
}

void DumpReport(int device, int type)
{
    const char* typeNames[] = { "Input", "Output", "Feature" };

    auto count = HID::Device_ReportCount(device, type);
    if (count < 0)
    {
        return;
    }

    std::cout << Indent(2).c_str() << typeNames[type] << " Report count " << count << "\n";
}

int main()
{
    int devices = HID::Devices_Open();
    if (devices)
    {
        int deviceCount = HID::Devices_Count(devices);

        std::cout << "Device Count: " << deviceCount << "\n\n";

        if (deviceCount)
        {
            for (int deviceIndex = 0; deviceIndex < deviceCount; ++deviceIndex)
            {
                int device = HID::Devices_OpenDevice(devices, deviceIndex);
                if (device && HID::Device_Opened(device))
                {
                    int vid = HID::Device_VID(device);
                    int pid = HID::Device_PID(device);

                    int msize = HID::Device_Manufacturer(device, nullptr, 0);
                    auto mfgr = std::make_unique<wchar_t[]>(msize);
                    HID::Device_Manufacturer(device, mfgr.get(), msize);
                    auto smfgr = std::make_unique<char[]>(msize);
                    wcstombs_s(nullptr, smfgr.get(), msize, mfgr.get(), msize);

                    int psize = HID::Device_Product(device, nullptr, 0);
                    auto product = std::make_unique<wchar_t[]>(psize);
                    HID::Device_Product(device, product.get(), psize);
                    auto sproduct = std::make_unique<char[]>(psize);
                    wcstombs_s(nullptr, sproduct.get(), psize, product.get(), psize);

                    std::cout << "VID " << std::hex << vid << "  PID " << pid << std::dec << "\n" <<
                        "   " << smfgr << " :: " << sproduct << "\n";

                    for (int i = 0; i < 3; ++i)
                    {
                        DumpReport(device, i);
                    }

                    std::cout << "\n";

                    HID::Device_Close(device);
                }
            }
            HID::Devices_Close(devices);
        }
    }
}
