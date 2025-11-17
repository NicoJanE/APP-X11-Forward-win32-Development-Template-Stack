 
/// Design Considerations and Coding Conventions (Click the links for details)
///-------------------------------------------------------------------------------------------------------------------------- 
//
//  - This code uses C standard types instead of Windows types. //[[doc/conventions/WindowsTypes-CTypes.md]]
//	- Headers are typically enclosed in namespaces; public functionality is separated as needed. /[[doc/conventions/NameSpaces.md]]  
//	- To use the static callback Window procedures with instance data, we use Dependency Injection or the Visitor pattern. [[doc/win32-callbacks-data.md]]

 

#ifndef SYS_WINDOWAPI_H
#define SYS_WINDOWAPI_H

#define WIN32_LEAN_AND_MEAN                     // Exclude rarely used stuff from the SDK
#include <windows.h>                            // SDK MingW

// C++ Includes
#include <type_traits> 


// **** Utility functions ****
namespace WindowNAPI
{
	/// Intent: Changes style after creating a window, maintain the same size, overwriting previous styles
	/// sample: ChangeWSStyle( pMainWin->GetHwnd(),   WS_HSCROLL );
	inline void ChangeWSStyle(HWND hWnd,  LONG_PTR pStyle, bool bUpdateTheStyle = true )
	{	
		LONG  currentWinStyle = 0;
		if(bUpdateTheStyle)
			currentWinStyle = GetWindowLong(hWnd, GWL_STYLE);

		RECT rWin;
		GetWindowRect(hWnd, &rWin);											// GetClientRect(hWnd, &rWin);
		SetWindowLongPtr(hWnd, GWL_STYLE, currentWinStyle | pStyle);		// GWL_STYLE make sure style will be updated
    
		// Some pStyles require the window to be resize with at least 1 pixel to be applied(i.e. WS_VSCROLL ands WS_VSCROLL)
		// we do this here and then we return back to original size ;(
		SetWindowPos(hWnd, HWND_TOPMOST, rWin.left, rWin.top, rWin.right-rWin.left , rWin.bottom-rWin.top+1, SWP_SHOWWINDOW);
		SetWindowPos(hWnd, HWND_TOPMOST, rWin.left, rWin.top, rWin.right-rWin.left , rWin.bottom-rWin.top, SWP_SHOWWINDOW);
	}

	/// Get HWND of the current window(hWnd)
	/// Sample: HWND value =(HWND) GetHwndParent(hDlg);
	inline  LONG WINAPI GetHwndParent( HWND hWnd) { return GetWindowLong(hWnd, GWLP_HWNDPARENT); };

	// Get ID of the window/control the lpClassName of createwindow()
	inline  LONG WINAPI GetWindowClassName( HWND hWnd) { return GetWindowLong(hWnd, GWL_ID); };




	// **** Template functions ****
	//__________________________________________________________________________________________________________________________________


	// Returns the Object PTR related to the parent HWND of the hCurrentWwnd parameter. this was store during creation of the window(SetWindowLongPtr see:  class::InitInstance() )
	// effective the member below is the same as:
		//	inline MainWin* GetObjectFromParentWnd( MainWin *pMainWin, HWND hCurrentWwnd)
		//	{
		//		HWND hwndOwner = GetParent(hCurrentWwnd);
		//		pMainWin = reinterpret_cast<MainWin*>(GetWindowLongPtr(hwndOwner, GWLP_USERDATA));
		//		return pMainWin;
		//	}

	// But this is the generic version which deduces the return type via the template variable(nice!)
	template <typename T>
	auto GetObjectFromParentWnd( T *pObjectOfWindow, HWND hCurrentWwnd) -> decltype(pObjectOfWindow)
	{
		T* Temp = pObjectOfWindow;
		HWND hwndOwner = GetParent(hCurrentWwnd);
		pObjectOfWindow = reinterpret_cast<T*>(GetWindowLongPtr(hwndOwner,GWLP_USERDATA));  
	
		// Test if the conversion is made to the correct type	
		//		1) Save the decltype of pObjectOfWindow before the reinterpret_cast instruction (Temp)
		//		2) Make the PTR(reinterpret_cast)
		//		3) compare decltype of 1 with decltype of 2 is it the same -> correct conversion (is_same)
		bool bYestheSame= false;
		if(std::is_same<decltype(Temp), decltype(pObjectOfWindow)>::value)		//if(std::is_same<int, int>::value)
			bYestheSame = true;		
		else		
			bYestheSame = false;
		
		return pObjectOfWindow;
	}
}

                  
namespace WNAPI = WindowNAPI;

#endif