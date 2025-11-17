 
/// Design Considerations and Coding Conventions (Click the links for details)
///-------------------------------------------------------------------------------------------------------------------------- 
//
//  - This code uses C standard types instead of Windows types. //[[doc/conventions/WindowsTypes-CTypes.md]]
//	- Headers are typically enclosed in namespaces; public functionality is separated as needed. /[[doc/conventions/NameSpaces.md]]  
//	- To use the static callback Window procedures with instance data, we use Dependency Injection or the Visitor pattern. [[doc/win32-callbacks-data.md]]

 

#ifndef SYS_WINDOWUTIL_H
#define SYS_WINDOWUTIL_H

#define WIN32_LEAN_AND_MEAN                   // Exclude rarely used stuff from the SDK
#include <windows.h>                          // SDK MingW

// C++ includes
#include <type_traits> 


// **** Utility functions ****
namespace WindowUTIL
{
	// For the rare cases we need to convert a unicode to ANSI/Multibyte. CALLER needs to free memory 
	inline LPCSTR ConvertWideCharToAnsi(const wchar_t* unicodeStr) 
	{
    	if (unicodeStr == nullptr) 
        	return nullptr;
    
    	int ansiStrSize = WideCharToMultiByte(CP_ACP, 0, unicodeStr, -1, NULL, 0, NULL, NULL);
    	char* ansiStr = new char[ansiStrSize];
    	WideCharToMultiByte(CP_ACP, 0, unicodeStr, -1, ansiStr, ansiStrSize, NULL, NULL);	// the conversion

    	return ansiStr;  
	}

}
namespace WNUTIL = WindowUTIL;

#endif