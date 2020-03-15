#pragma once

#include <string.h>
#include <cstdio>
#include "List.hpp"
#include "w32.hpp"
#include "w32_video.hpp"
#include "VfwDriver.hpp"




class VfwCaptureDevice
{
    int fDriverIndex;
    bool fIsConnected;
    int fFramesPerSecond;           // frames per second
    int fInterval;      // interval between frames in microseconds
    int fWidth;
    int fHeight;
    HWND fWindowHandle;

    CAPSTATUS fCaptureStatus;
    CAPDRIVERCAPS fDriverCapabilities;
    VfwDeviceDriver fDeviceDriver;

public:

    VfwCaptureDevice(int index, int width, int height, int fps)
    {
        fDriverIndex = index;
        fIsConnected = false;
        fWidth = width;
        fHeight = height;

        CreateWindow(0, 0, width, height);
        setFramesPerSecond(fps);

        Connect();
    }

    bool CreateWindow(int x, int y, int width, int height)
    {
        char name[100];
        sprintf_s(name, 100, "VfwCaptureWindow: %d", fDriverIndex);
        fWindowHandle = capCreateCaptureWindowA(name, WS_VISIBLE, x, y, width, height, nullptr, fDriverIndex);
    
        return true;
    }

    bool Connect()
    {
        // If we're already connected, then simply return
        if (fIsConnected) {
            return true;
        }

        fIsConnected = this->SendMessage(WM_CAP_DRIVER_CONNECT, fDriverIndex, 0) == 1;

        if (!fIsConnected) {
            return false;
            //throw new Exception("VfwCaptureDevice - Connection to device failed");
        }

        // Get the driver capabilities
        bool gotDriverCaps = this->DriverGetCaps(fDriverCapabilities);

        // Finally allow a subclasser to do something special
        // upon connection.
        OnConnected();

        GetStatus(fCaptureStatus);
    
        char dName[100];
        char vName[100];


        DriverGetName(dName, 100);
        DriverGetVersion(vName, 100);
        fDeviceDriver.setName(dName);
        fDeviceDriver.setVersion(vName);

        return fIsConnected;
    }

    bool isConnected() {return fIsConnected;}

    void setFramesPerSecond(const int fps)
    {
        fFramesPerSecond = fps;
        fInterval = (int)((1.0 / fps * 1000000.0) + 0.5);  // We specify intervals in microseconds

        // We're not capturing in a steady stream, but 
        // we do need to make sure the capture configuration
        // is reset
        ConfigureCapture();
    }

/*
    int FramePerSecond
    {
            get { return fFps; }
            protected set
            {
                fFps = value;
                fInterval = (int)((1.0 / fFps * 1000000.0) + 0.5);  // We specify intervals in microseconds
            }
    }
*/

    virtual void OnConnected() {}

    void Disconnect()
    {
        OnDisconnecting();

        if (fIsConnected) {
            DriverDisconnect();
        }

        fIsConnected = false;
    }

    virtual void OnDisconnecting()
    {
    }

    virtual void ConfigureCapture() {}


    int getCaptureInterval() {return fInterval; }
    int getWidth() {return fWidth;}
    int getHeight() {return fHeight;}
    int getDriverIndex() {return fDriverIndex;}




/*
    bool SetCallbackOnError(VfwcapErrorCallback fpProc)
    {
        bool success = 1 == SendCallbackMessage(fWindowHandle, WM_CAP_SET_CALLBACK_ERROR, 0, fpProc);
        return success;
    }

    bool SetCallbackOnStatus(VfwcapStatusCallback fpProc)
    {
        return 1 == SendCallbackMessage(fWindowHandle, WM_CAP_SET_CALLBACK_STATUS, 0, fpProc);
    }
*/
        //public bool capSetCallbackOnYield(Vfw.CallBackDelegate fpProc)
        //{
        //    return 1 == SendCallbackMessage(fWindowHandle, WM_CAP_SET_CALLBACK_YIELD, 0, fpProc);
        //}
/*
    bool SetCallbackOnFrame(VfwVideoFrameDelegate fpProc)
    {
        return 1 == SendCallbackMessage(fWindowHandle, WM_CAP_SET_CALLBACK_FRAME, 0, fpProc);
    }

    bool SetCallbackOnVideoStream(VfwVideoFrameDelegate fpProc)
    {
        bool success = 1 == SendCallbackMessage(fWindowHandle, WM_CAP_SET_CALLBACK_VIDEOSTREAM, 0, fpProc);
        return success;
    }
*/
        //public bool capSetCallbackOnWaveStream(Vfw.CallBackDelegate fpProc)
        //{
        //    return 1 == SendMessageA(fWindowHandle, WM_CAP_SET_CALLBACK_WAVESTREAM, 0, fpProc);
        //}
/*
    bool SetCallbackOnCapControl(VfwcapControlCallback fpProc)
    {
        return 1 == SendCallbackMessage(fWindowHandle, WM_CAP_SET_CALLBACK_CAPCONTROL, 0, fpProc);
    }
*/


    bool SetUserData(int lUser)
    {
        return 1 ==  SendMessage(WM_CAP_SET_USER_DATA, 0, lUser);
    }

    bool GetUserData()
    {
        return 1 ==  SendSimpleMessage(WM_CAP_GET_USER_DATA);
    }

    bool DriverConnect()
    {
        return 1 ==  SendMessage(WM_CAP_DRIVER_CONNECT, fDriverIndex, 0);
    }

    bool DriverDisconnect()
    {
        return 1 ==  SendSimpleMessage(WM_CAP_DRIVER_DISCONNECT);
    }

    bool DriverGetName(char *szName, int wSize)
    {
        return 1 == SendMessageA(fWindowHandle, WM_CAP_DRIVER_GET_NAME, wSize, (LPARAM)szName);
    }

    bool DriverGetVersion(char * szVer, int wSize)
    {
        return 1 == SendMessageA(fWindowHandle, WM_CAP_DRIVER_GET_VERSION, wSize, (LPARAM)szVer);
    }

    bool DriverGetCaps(CAPDRIVERCAPS &s)
    {
        int wSize = sizeof(s);
        bool success = (1 == SendMessageA(fWindowHandle, WM_CAP_DRIVER_GET_CAPS, wSize, (LPARAM)&s));
        return success;
    }

    bool FileSetCaptureFile(const char * szName)
    {
        bool success = (1 == SendMessageA(fWindowHandle, WM_CAP_FILE_SET_CAPTURE_FILE, 0, (LPARAM)szName));
        return success;
    }

    bool FileGetCaptureFile(char * szName, int wSize)
    {
        return 1 == SendMessageA(fWindowHandle, WM_CAP_FILE_GET_CAPTURE_FILE, wSize, (LPARAM)szName);
    }

    bool FileAlloc(int dwSize)
    {
        return 1 == SendMessage(WM_CAP_FILE_ALLOCATE, 0, dwSize);
    }

    bool FileSaveAs(const char * szName)
    {
        return 1 == SendMessageA(fWindowHandle, WM_CAP_FILE_SAVEAS, 0, (LPARAM)szName);
    }

    bool FileSetInfoChunk(CAPINFOCHUNK &infoChunk)
    {
        return 1 == SendMessageA(fWindowHandle, WM_CAP_FILE_SET_INFOCHUNK, 0, (LPARAM)&infoChunk);
    }

    bool FileSaveDIB(const char *szName)
    {
        return 1 == SendMessageA(fWindowHandle, WM_CAP_FILE_SAVEDIB, 0, (LPARAM)szName);
    }

    bool EditCopy()
    {
        return 1 == SendSimpleMessage(WM_CAP_EDIT_COPY);
    }



    bool SetAudioFormat(WAVEFORMATEX &s, int wSize)
    {
        return 1 == SendMessageA(fWindowHandle, WM_CAP_SET_AUDIOFORMAT, wSize, (LPARAM)&s);
    }

    int GetAudioFormat(WAVEFORMATEX &s, int wSize)
    {
        int retValue = SendMessageA(fWindowHandle, WM_CAP_GET_AUDIOFORMAT, wSize, (LPARAM)&s);
        return retValue;
    }

    int GetAudioFormatSize()
    {
        int retValue = SendSimpleMessage(WM_CAP_GET_AUDIOFORMAT);
        return retValue;
    }



    /// <summary>
    /// Retrieves a copy of the video format information.
    /// </summary>
    /// <param name="s"></param>
    /// <returns>Returns 0 if not connected to driver.</returns>
    int GetVideoFormat(BITMAPINFO &s)
    {
        int wSize = sizeof(s);
        int  bytesCopied = SendMessageA(fWindowHandle, WM_CAP_GET_VIDEOFORMAT, wSize, (LPARAM)&s);

        return bytesCopied;
    }

    /// <summary>
    /// Retrieve the size needed to hold the data structure representing the video format
    /// information.
    /// </summary>
    /// <returns></returns>
    int GetVideoFormatSize()
    {
        int retValue = SendSimpleMessage(WM_CAP_GET_VIDEOFORMAT);
        return retValue;
    }

    bool SetVideoFormat(BITMAPINFO &s)
    {
        int wSize = sizeof(s);
        bool success = 1 == SendMessageA(fWindowHandle, WM_CAP_SET_VIDEOFORMAT, wSize, (LPARAM)&s);
        return success;
    }



    bool DlgVideoFormat()
    {
        return 1 == SendSimpleMessage(WM_CAP_DLG_VIDEOFORMAT);
    }

    bool DlgVideoSource()
    {
        return 1 == SendSimpleMessage(WM_CAP_DLG_VIDEOSOURCE);
    }

    bool DlgVideoDisplay()
    {
        return 1 == SendSimpleMessage(WM_CAP_DLG_VIDEODISPLAY);
    }

    bool DlgVideoCompression()
    {
        return 1 == SendSimpleMessage(WM_CAP_DLG_VIDEOCOMPRESSION);
    }



    bool Preview(bool f)
    {
        int BOOL = f == true ? 1 : 0;
        return 1 ==  SendMessage(WM_CAP_SET_PREVIEW, BOOL, 0);
    }

    bool PreviewRate(int wMS)
    {
        return 1 == SendMessage(WM_CAP_SET_PREVIEWRATE, wMS, 0);
    }

    bool PreviewScale(bool f)
    {
        int BOOL = f == true ? 1 : 0;
        return 1 == SendMessage(WM_CAP_SET_SCALE, (BOOL), 0);
    }

    bool Overlay(bool f)
    {
        int BOOL = f == true ? 1 : 0;
        return 1 == SendMessage(WM_CAP_SET_OVERLAY, (BOOL), 0);
    }

    /// <summary>
    /// Grab a single frame from the device.  Overlay and preview are stopped if they
    /// are currently active.
    /// </summary>
    /// <returns></returns>
    bool GrabFrame()
    {
        return 1 == SendSimpleMessage(WM_CAP_GRAB_FRAME);
    }

    /// <summary>
    /// Grab a single from from the device.  Overlay and Preview are not stopped.
    /// </summary>
    /// <returns></returns>
    bool GrabFrameNoStop()
    {
        return 1 == SendSimpleMessage(WM_CAP_GRAB_FRAME_NOSTOP);
    }



    bool GetStatus(CAPSTATUS &s)
    {
        int wSize = sizeof(s);
        bool success = 1 == SendMessageA(fWindowHandle, WM_CAP_GET_STATUS, wSize, (LPARAM)&s);
        return success;
    }

    bool SetScrollPos(POINT &lpP)
    {
        return 1 == SendMessageA(fWindowHandle, WM_CAP_SET_SCROLL, 0, (LPARAM)&lpP);
    }

    bool CaptureSequence()
    {
        return 1 == SendSimpleMessage(WM_CAP_SEQUENCE);
    }

    bool CaptureSequenceNoFile()
    {
        return 1 == SendSimpleMessage(WM_CAP_SEQUENCE_NOFILE);
    }

    bool CaptureStop()
    {
        return 1 == SendSimpleMessage(WM_CAP_STOP);
    }

    bool CaptureAbort()
    {
        return 1 == SendSimpleMessage(WM_CAP_ABORT);
    }

    bool CaptureSingleFrameOpen()
    {
        return 1 == SendSimpleMessage(WM_CAP_SINGLE_FRAME_OPEN);
    }

    bool CaptureSingleFrameClose()
    {
        return 1 == SendSimpleMessage(WM_CAP_SINGLE_FRAME_CLOSE);
    }

    bool CaptureSingleFrame()
    {
        return 1 == SendSimpleMessage(WM_CAP_SINGLE_FRAME);
    }

    bool CaptureGetSetup(CAPTUREPARMS &s)
    {
        size_t wSize = sizeof(s);
        return 1 == SendMessageA(fWindowHandle, WM_CAP_GET_SEQUENCE_SETUP, wSize, (LPARAM)&s);
    }

    bool CaptureSetSetup(CAPTUREPARMS &s)
    {
        size_t wSize = sizeof(s);
        return 1 == SendMessageA(fWindowHandle, WM_CAP_SET_SEQUENCE_SETUP, wSize, (LPARAM)&s);
    }

    bool SetMCIDeviceName(char * szName)
    {
        return 1 == SendMessageA(fWindowHandle, WM_CAP_SET_MCI_DEVICE, 0, (LPARAM)szName);
    }

    bool GetMCIDeviceName(char* szName, int wSize)
    {
        return 1 == SendMessageA(fWindowHandle, WM_CAP_GET_MCI_DEVICE, (wSize), (LPARAM)szName);
    }

    bool PaletteOpen(char * szName)
    {
        return 1 == SendMessageA(fWindowHandle, WM_CAP_PAL_OPEN, 0, (LPARAM)szName);
    }

    bool PaletteSave(char * szName)
    {
        return 1 == SendMessageA(fWindowHandle, WM_CAP_PAL_SAVE, 0, (LPARAM)szName);
    }

    bool PalettePaste()
    {
        return 1 == SendSimpleMessage(WM_CAP_PAL_PASTE);
    }

    bool PaletteAuto(int iFrames, int &iColors)
    {
        return 1 ==  SendMessage(WM_CAP_PAL_AUTOCREATE, iFrames, (LPARAM)iColors);
    }

    bool PaletteManual(int fGrab, int iColors)
    {
        return 1 ==  SendMessage(WM_CAP_PAL_MANUALCREATE, fGrab, iColors);
    }
    
    // Send a message that doesn't have any parameters
    int SendSimpleMessage(int msg)
    {
        int retValue = SendMessageA(fWindowHandle, msg, (WPARAM)0, (LPARAM)0);
        return retValue;
    }

    int SendMessage(int msg, WPARAM wParam, LPARAM lParam)
    {
        int retValue = SendMessageA(fWindowHandle, msg, (WPARAM)wParam, (LPARAM)lParam);
        return retValue;
    }
};

class VfwDeviceManager
{
    static const int gmaxDevices = 10;    // Values range from 0 - 9 according to the Vfw documentation

    VfwDeviceDriver devices[10];

public:
    VfwDeviceManager()
    {
        Refresh();
    }

    void Refresh()
    {
        char name[100];
        char version[100];

        for (int i = 0; i < gmaxDevices; i++)
        {
            if (capGetDriverDescriptionA(i, name, 100, version, 100))
            {
                devices[i].index = i;
                devices[i].setName(name);
                devices[i].setVersion(version);
            }
        }
    }


    VfwDeviceDriver & getDevice(const int index)
    {
        return devices[index];
    }

    VfwDeviceDriver & operator[] (const int index)
    {
        return devices[index];
    }
};
