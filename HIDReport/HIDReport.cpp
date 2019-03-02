#include "stdafx.h"

#define HID_IMPORT_EXPORT _declspec(dllexport) _stdcall 
#include "HIDReport.h"


#pragma once

#ifndef HID_IMPORT_EXPORT
#define HID_IMPORT_EXPORT _declspec(dllimport) _stdcall 
#endif

extern "C"
{

    int HID_IMPORT_EXPORT Devices_Open()
    {

    }

    int HID_IMPORT_EXPORT Devices_Count(int handle)
    {

    }

    int HID_IMPORT_EXPORT Devices_OpenDevice(int handle, int index)
    {

    }

    void HID_IMPORT_EXPORT Devices_Close(int handle)
    {

    }


    void HID_IMPORT_EXPORT Device_Close(int handle)
    {

    }

    int HID_IMPORT_EXPORT Device_ReportCount(int handle, int type)
    {

    }

    int HID_IMPORT_EXPORT Device_OpenReportCollection(int handle, int type, int id)
    {

    }

    int HID_IMPORT_EXPORT Device_Name(int handle, wchar_t* name, int nameSize)
    {

    }

    int HID_IMPORT_EXPORT Device_VID(int handle)
    {

    }

    int HID_IMPORT_EXPORT Device_PID(int handle)
    {

    }



    void HID_IMPORT_EXPORT Collection_Close(int handle)
    {

    }

    int HID_IMPORT_EXPORT Collection_Usage(int handle)
    {

    }

    int HID_IMPORT_EXPORT Collection_CollectionCount(int handle)
    {

    }

    int HID_IMPORT_EXPORT Collection_OpenCollection(int handle, int index)
    {

    }

    int HID_IMPORT_EXPORT Collection_ButtonCount(int handle)
    {

    }

    int HID_IMPORT_EXPORT Collection_ButtonUsage(int handle, int index)
    {

    }

    int HID_IMPORT_EXPORT Collection_ButtonState(int handle, int index)
    {
        // TODO: Implement me
        return 0;
    }

    int HID_IMPORT_EXPORT Collection_ValueCount(int handle)
    {

    }

    int HID_IMPORT_EXPORT Collection_ValueUsage(int handle, int index)
    {

    }

    int HID_IMPORT_EXPORT Collection_ValueValue(int handle, int index)
    {
        // TODO: Implement me
        return 0;
    }

    int HID_IMPORT_EXPORT VID_Name(int vid, wchar_t* name, int nameSize)
    {
        // TODO: Implement me
        return 0;
    }

    int HID_IMPORT_EXPORT PID_Name(int vid, int pid, wchar_t* name, int nameSize)
    {
        // TODO: Implement me
        return 0;
    }

    int HID_IMPORT_EXPORT Usage_Name(int usage)
    {
        // TODO: Implement me
        return 0;
    }
}