 
/// Design Considerations and Coding Conventions (Click the links for details)
///-------------------------------------------------------------------------------------------------------------------------- 
//
//  - This code uses C standard types instead of Windows types. //[[doc/conventions/WindowsTypes-CTypes.md]]
//	- Headers are typically enclosed in namespaces; public functionality is separated as needed. /[[doc/conventions/NameSpaces.md]]  
//	- To use the static callback Window procedures with instance data, we use Dependency Injection or the Visitor pattern. [[doc/win32-callbacks-data.md]]

 
#ifndef STARTUP_H
#define STARTUP_H

#define WIN32_LEAN_AND_MEAN                     // Exclude rarely used stuff from the SDK
#include <windows.h>                            // SDK MingW
//#include "../../../headers/views/_main/WinMain.h"
#include "WinMain.h"


// Forward declarations of functions included in this code module:
LRESULT static CALLBACK    WndProcDispatcher(HWND, UINT, WPARAM, LPARAM);				// Note static because we are going to route/dispatch to the correct WndProc(the one of the class object) via this function
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


 
#endif
