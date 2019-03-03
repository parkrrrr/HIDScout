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

void DumpCollection(int collection, int indent)
{
    auto subCollectionCount = HID::Collection_CollectionCount(collection);
    auto buttonCount = HID::Collection_ButtonCount(collection);
    auto valueCount = HID::Collection_ValueCount(collection);

    std::cout << Indent(indent).c_str() << "Usage " << std::hex << HID::Collection_Usage(collection) << std::dec << "\n";

    std::cout << Indent(indent).c_str();
    if (buttonCount) std::cout << buttonCount << " buttons ";
    if (valueCount) std::cout << valueCount << " values ";
    if (subCollectionCount) std::cout << subCollectionCount << " subcollections ";
    std::cout << "\n";

    for (int buttonIndex = 0; buttonIndex < buttonCount; ++buttonIndex)
    {
        std::cout << Indent(indent).c_str() << "Button " << std::hex << HID::Collection_ButtonUsage(collection, buttonIndex) << std::dec << "\n";
    }

    for (int valueIndex = 0; valueIndex < valueCount; ++valueIndex)
    {
        std::cout << Indent(indent).c_str() << "Value  " << std::hex << HID::Collection_ValueUsage(collection, valueIndex) << std::dec << "\n";
    }

    for (int collectionIndex = 0; collectionIndex < subCollectionCount; ++collectionIndex)
    {
        auto subCollection = HID::Collection_OpenCollection(collection, collectionIndex);
        if (subCollection)
        {
            std::cout << Indent(indent).c_str() << "----------------------------------\n";
            DumpCollection(subCollection, indent + 1);
            HID::Collection_Close(subCollection);
        }
    }
}

void DumpReport(int device, int type)
{
    const char* typeNames[] = { "Input", "Output", "Feature" };

    auto count = HID::Device_ReportCount(device, type);
    if (count < 0)
    {
        return;
    }

    std::cout << Indent(2).c_str() << typeNames[type] << " Max Report ID " << count << "\n";

    int startID = 0;
    int endID = 0;
    if (count > 0)
    {
        startID = 1;
        endID = count;
    }

    for (int id = startID; id <= endID; ++id)
    {
        auto collection = HID::Device_OpenReportCollection(device, type, id);
        if (collection)
        {
            auto subCollectionCount = HID::Collection_CollectionCount(collection);
            auto buttonCount = HID::Collection_ButtonCount(collection);
            auto valueCount = HID::Collection_ValueCount(collection);
            if (subCollectionCount || buttonCount || valueCount)
            {
                std::cout << Indent(3).c_str() << "Report " << id << "\n";
                DumpCollection(collection, 4);
            }
            HID::Collection_Close(collection);
        }
    }
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
