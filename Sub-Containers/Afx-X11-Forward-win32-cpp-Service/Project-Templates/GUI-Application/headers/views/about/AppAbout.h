 
/// Design Considerations and Coding Conventions (Click the links for details)
///-------------------------------------------------------------------------------------------------------------------------- 
//
//  - This code uses C standard types instead of Windows types. //[[doc/conventions/WindowsTypes-CTypes.md]]
//	- Headers are typically enclosed in namespaces; public functionality is separated as needed. /[[doc/conventions/NameSpaces.md]]  
//	- To use the static callback Window procedures with instance data, we use Dependency Injection or the Visitor pattern. [[doc/win32-callbacks-data.md]]

 

#ifndef APPABOUT_H
#define APPABOUT_H

#define WIN32_LEAN_AND_MEAN                     // Exclude rarely used stuff from the SDK
#include <windows.h>                            // SDK MingW

class AppAbout
{
public:
	AppAbout(){}
	~AppAbout(){}

public:


	
	static  INT_PTR CALLBACK CB_About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
};

#endif