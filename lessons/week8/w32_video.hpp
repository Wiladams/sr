#pragma once

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "vfw32.lib")

#ifdef __cplusplus
extern "C" {            /* Assume C declarations for C++ */
#endif  /* __cplusplus */

DWORD FAR PASCAL VideoForWindowsVersion(void);

LONG VFWAPI InitVFW(void);
LONG VFWAPI TermVFW(void);

#ifdef __cplusplus
} // extern "C"
#endif  /* __cplusplus */

#define AVICapSM(hwnd,m,w,l) ( (IsWindow(hwnd)) ?   SendMessage(hwnd,m,w,l) : 0)
