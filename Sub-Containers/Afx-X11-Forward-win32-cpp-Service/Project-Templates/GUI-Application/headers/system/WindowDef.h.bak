#ifndef SYS_WINDOWDEF_H
#define SYS_WINDOWDEF_H

#define WIN32_LEAN_AND_MEAN                     // Exclude rarely used stuff from the SDK
#include <windows.h>                            // SDK MingW


namespace WindowNAPI
{
	// Use GetLastError() in case this returns NULL, send:  WM_NCCREATE, WM_NCCALCSIZE, and WM_CREATE message  when created
	// Note: The CS_ apply for all instances (hence class style). This in contrast with 'Windows styles'(WS_) and 'Extended window styles'(WS_EX_) 
	// which account for each instance(see:CreateWindowEx )
	inline HWND WINAPI CreateWindowExnje(
	  _In_     DWORD     dwExStyle,					// Extended Window Styles(per instance!).Default: WS_EX_LEFT								->	https://docs.microsoft.com/en-us/windows/desktop/winmsg/extended-window-styles
	  _In_opt_ LPCTSTR   lpClassName,				// class ID  name set in: WNDCLASSEXW. lpszClassName
	  _In_opt_ LPCTSTR   lpWindowName,				// Visible Title in window or control name
	  _In_     DWORD     dwStyle,					// Windows styles(BUTTON, COMBOBOX,	EDIT, LISTBOX, RICHEDIT_CLASS, SCROLLBAR, STATIC)		->	https://msdn.microsoft.com/en-us/library/windows/desktop/ms632600(v=vs.85).aspx
	  _In_     int       x,							// x Position (CW_USEDEFAULT)
	  _In_     int       y,							// Y Position (CW_USEDEFAULT)
	  _In_     int       nWidth,					// Width (device units), overlapped => screen coordinates, CW_USEDEFAULT 
	  _In_     int       nHeight,					// Height (device units), overlapped => screen coordinates, CW_USEDEFAULT 
	  _In_opt_ HWND      hWndParent,				// Handle to parent (optional for pop-up). for message-only window: HWND_MESSAGE (https://docs.microsoft.com/en-us/windows/desktop/winmsg/window-features#message-only-windows)
	  _In_opt_ HMENU     hMenu,						// overlapped : reference to menu, child controls: ID of control to notify events
	  _In_opt_ HINSTANCE hInstance,					// handle to the instance 
	  _In_opt_ LPVOID    lpParam
	)
	{
		return ::CreateWindowEx(dwExStyle,  lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, hInstance);
	
	}


	// 1 Replace this with a rect def
	/*
	 _In_     int       x,							// x Position (CW_USEDEFAULT)
	  _In_     int       y,							// Y Position (CW_USEDEFAULT)
	  _In_     int       nWidth,					// Width (device units), overlapped => screen coordinates, CW_USEDEFAULT 
	  _In_     int       nHeight,					// Height (device units), overlapped => screen coordinates, CW_USEDEFAULT

	  */
}
namespace WNAPI = WindowNAPI;


#endif


