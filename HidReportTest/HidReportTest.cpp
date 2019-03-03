// HidReportTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

namespace HID
{
#include "..\HIDReport\HIDReport.h"
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

                    std::cout << "VID " << std::hex << vid << "  PID " << pid << "\n" <<
                        "   " << smfgr << " :: " << sproduct << "\n\n";

                    HID::Device_Close(device);
                }
            }
            HID::Devices_Close(devices);
        }
    }
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
