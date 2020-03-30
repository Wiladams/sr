Week 9 - Focus on Network programming

Compiling an application
========================

This week we pull in some C++ specifics that require additional flags to compile

c:> cl /EHsc /I.. filename.cpp

There is this additional '/EHsc' flag that needs to be added.  It is specific to 
support the std::queue collection object.

new Win32 APIs
--------------
w32_socket
w32_video

New Classes
-----------
VfwCameraDevice.cpp
guistyle
PBDIBSection
PixelBuffer24

New Demos
---------
snapserver
snapclient
vidcam
vidcap
vidplayground
vidview
