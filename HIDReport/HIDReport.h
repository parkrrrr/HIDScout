#pragma once

#ifndef HID_IMPORT_EXPORT
#define HID_IMPORT_EXPORT _declspec(dllimport) _stdcall 
#endif
/*

    Usages

    In this document, the integer representing a "usage" is a 16-bit integer.
    The high byte (bits 8-15) represents the usage page.
    The low byte (bits 0-7) represents the usage within that page.
    This is consistent with the USB HID class specification.

*/

extern "C"
{
    // **** Devices is a collection of the attached HID devices.

    // Open the Devices collection
    // Returns a handle to the opened collection
    int HID_IMPORT_EXPORT Devices_Open();

    // Get the number of available devices
    // handle = the handle from Devices_Open
    // Returns the number of devices
    int HID_IMPORT_EXPORT Devices_Count(int handle);

    // Open a device. 
    // handle = the handle from Devices_Open
    // index = the device to open 0 <= index < Devices_Count
    // Returns a handle to the opened device
    int HID_IMPORT_EXPORT Devices_OpenDevice(int handle, int index);

    // Close a Devices collection
    // handle = the handle from Devices_Open
    void HID_IMPORT_EXPORT Devices_Close(int handle);


    // **** Device is an opened device

    // Close a device
    // handle = the handle from Devices_OpenDevice
    void HID_IMPORT_EXPORT Device_Close(int handle);

    // Determine whether the device was successfully opened
    // handle = the handle from Devices_OpenDevice
    // Returns 1 if the device was successfully opened, 0 otherwise
    // A device cannot be opened if another application has opened it for exclusive access.
    // For example, my Wacom Intuos S tablet can only be opened if I stop the Wacom driver service first.
    int HID_IMPORT_EXPORT Device_Opened(int handle);

    // Get the number of supported reports of a given type
    // handle = the handle from Devices_OpenDevice
    // type = input=0, output=1, feature=2
    // Returns the number of reports, sort of.
    // If there is exactly one report, but report IDs are not used for this type, this method returns 0.
    // If there are no reports of this type, this method returns -1.
    int HID_IMPORT_EXPORT Device_ReportCount(int handle, int type);

    // Open the specified report
    // handle = the handle from Devices_OpenDevice
    // type = input=0, output=1, feature=2
    // id = 
    //       0 if Device_ReportCount is 0
    //       1 <= id <= ReportCount otherwise
    // Returns a handle to the opened report's top-level collection
    int HID_IMPORT_EXPORT Device_OpenReportCollection(int handle, int type, int id);

    // Get the manufacturer name of the device
    // handle = the handle from Devices_OpenDevice
    // name = a caller-allocated buffer
    // nameSize = the size of name in characters (WORDs)
    // Returns
    //        the necessary size in characters if name is nullptr, including the null temrinator
    //        the number of characters written otherwise, including the null terminator
    int HID_IMPORT_EXPORT Device_Manufacturer(int handle, wchar_t* name, int nameSize);

    // Get the product name of the device
    // handle = the handle from Devices_OpenDevice
    // name = a caller-allocated buffer
    // nameSize = the size of name in characters (WORDs)
    // Returns
    //        the necessary size in characters if name is nullptr, including the null temrinator
    //        the number of characters written otherwise, including the null terminator
    int HID_IMPORT_EXPORT Device_Product(int handle, wchar_t* name, int nameSize);

    // Get the VID (vendor ID) of the device
    // handle = the handle from Devices_OpenDevice
    // Returns the VID (vendor ID) of the device
    int HID_IMPORT_EXPORT Device_VID(int handle);

    // Get the PID (product ID) of the device
    // handle = the handle from Devices_OpenDevice
    // Returns the PID (product ID) of the device
    int HID_IMPORT_EXPORT Device_PID(int handle);


    // **** Collection is an opened collection of controls

    // Close an opened collection
    // handle = the collection handle from Device_OpenReportCollection or Collection_OpenCollection
    void HID_IMPORT_EXPORT Collection_Close(int handle);

    // Get the usage for the collection
    // handle = the collection handle from Device_OpenReportCollection or Collection_OpenCollection
    // Returns the usage of the collection (See Usages, above)
    int HID_IMPORT_EXPORT Collection_Usage(int handle);

    // Get the number of sub-collections of this collection
    // handle = the collection handle from Device_OpenReportCollection or Collection_OpenCollection
    // Returns the number of sub-collections
    int HID_IMPORT_EXPORT Collection_CollectionCount(int handle);

    // Open a sub-collection of this collection
    // handle = the collection handle from Device_OpenReportCollection or Collection_OpenCollection
    // index ->    0 <= index < Collection_CollectionCount
    int HID_IMPORT_EXPORT Collection_OpenCollection(int handle, int index);

    // Get the number of buttons in this collection
    // handle = the collection handle from Device_OpenReportCollection or Collection_OpenCollection
    // Returns the number of buttons in this report
    int HID_IMPORT_EXPORT Collection_ButtonCount(int handle);

    // Get the Usage of the specified button
    // handle = the collection handle from Device_OpenReportCollection or Collection_OpenCollection
    // index ->   0 <= index < Collection_ButtonCount
    // Returns the usage of the button
    int HID_IMPORT_EXPORT Collection_ButtonUsage(int handle, int index);

    // Get the state of the specified button
    // handle = the collection handle from Device_OpenReportCollection or Collection_OpenCollection
    // index ->   0 <= index < Collection_ButtonCount
    // Returns the current state of the button
    // Note: this method is only meaningful for Input reports
    // Note: this method is currently unimplemented
    int HID_IMPORT_EXPORT Collection_ButtonState(int handle, int index);

    // Get the number of non-button values in this collection
    // handle = the collection handle from Device_OpenReportCollection or Collection_OpenCollection
    // Returns the number of non-button values in this report
    int HID_IMPORT_EXPORT Collection_ValueCount(int handle);

    // Get the usage of the specified value
    // handle = the collection handle from Device_OpenReportCollection or Collection_OpenCollection
    // index ->   0 <= index < Collection_ValueCount
    // Returns the usage of the value
    int HID_IMPORT_EXPORT Collection_ValueUsage(int handle, int index);

    // Get the current value of the specified value
    // handle = the collection handle from Device_OpenReportCollection or Collection_OpenCollection
    // index ->   0 <= index < Collection_ValueCount
    // Note: this method is only meaningful for Input reports
    // Note: this method is currently unimplemented
    int HID_IMPORT_EXPORT Collection_ValueValue(int handle, int index);


    // **** Support functions

    // Get the vendor name that corresponds to a given Vendor ID (VID)
    // vid = the VID to retrieve
    // name = a caller-allocated buffer to receive the name
    // nameSize = the size of name in characters
    // Returns
    //          0 if the VID is unrecognized
    //          The necessary size in characters including the null terminator, if name==nullptr
    //          The number of characters copied including the null terminator, otherwise
    // Note: this method is currently unimplemented
    int HID_IMPORT_EXPORT VID_Name(int vid, wchar_t* name, int nameSize);

    // Get the product name that corresponds to a given vendor and product ID (VID/PID) pair
    // vid = the VID of the product to retrieve
    // pid = the PID of the product to retrieve
    // name = a caller-allocated buffer to receive the product name
    // nameSize = the size of name in characters
    // Returns
    //          0 if the VID/PID is unrecognized
    //          The necessary size in characters including the null terminator, if name==nullptr
    //          The number of characters copied including the null terminator, otherwise
    // Note: this method is currently unimplemented
    int HID_IMPORT_EXPORT PID_Name(int vid, int pid, wchar_t* name, int nameSize);

    // Get the name that corresponds to the given usage
    // usage = the usage to retrieve (see Usages, above)
    // name = a caller-allocated buffer to receive the usage name
    // nameSize = the size of name in characters
    // Returns
    //          0 if the usage is unrecognized
    //          The necessary size in characters including the null terminator, if name==nullptr
    //          The number of characters copied including the null terminator, otherwise
    // Note: this method is currently unimplemented
    int HID_IMPORT_EXPORT Usage_Name(int usage);
}