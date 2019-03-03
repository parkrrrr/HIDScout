# HIDScout

## Rationale

In the process of developing hardware to implement a new USB HID usage page, I came to a realization: 
There's no current hardware that implements this usage page, so I have no way to test any software I might 
write. And there's no software that implements this usage page, so I have no way to test the hardware I'm
developing. 

In order to break out of that infinite loop, I decided to write a general-purpose utility that'll allow me
to look at the data reported by any HID class device, subject to operating system restrictions. 

## Features

Currently, HIDScout itself is not yet doing anything useful. It's a Win32 app with a XAML UI, taking advantage 
of the new [XAML Islands](https://docs.microsoft.com/en-us/windows/uwp/xaml-platform/xaml-host-controls) feature 
in the [Windows 10 October 2018 Update](https://support.microsoft.com/gl-es/help/4028685/windows-10-get-the-update), 
and I'm still learning my way around that particular frontier. However, all is not lost. If you need to see some 
HID data today, see *HIDReportTest* below.

## HIDReport

*HIDReport* is a Windows DLL that exposes information about the report structure of attached USB HID devices. 
It's still in its infancy, but it is currently capable of providing information about any USB HID device that 
can be opened. For now, that's just some identifying information, the heirarchical structure, and the usages 
of the controls on each device, but it'll likely be expanded in the future to include current states of all 
input controls as well as more static information (such as the size of values and whether they are relative or
absolute.)

It also contains a few unimplemented APIs that will provide human-readable descriptions of well-defined HID 
usages, plus an interface to the [Linux USB ID Repository](http://www.linux-usb.org/usb-ids.html).

## HIDReportTest

*HIDReportTest* is just what it says - an application to test the functionality of *HIDReport*. However, it's
also a completely functional application. When you run it from a command prompt, it will dump the entire HID
report tree for every attached device it can open.

## Troubleshooting

If you have an attached device that you'd expect to see in the listing, you may find that you have to stop 
a service or some other software that's currently attached to that device in exclusive mode. For example, to
see my Wacom Intuos tablet, I have to stop the Wacom service. (Sadly, all of the usages it reports are 
vendor-specific, so there's no particular joy in doing so.)

