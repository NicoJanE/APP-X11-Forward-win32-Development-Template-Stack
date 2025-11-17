 
/// Design Considerations and Coding Conventions (Click the links for details)
///-------------------------------------------------------------------------------------------------------------------------- 
//
//  - This code uses C standard types instead of Windows types. [[doc/conventions/WindowsTypes-CTypes.md]]
//	- Headers are typically enclosed in namespaces; public functionality is separated as needed. [[doc/conventions/NameSpaces.md]]  
//	- To use the static callback Window procedures with instance data, we use Dependency Injection or the Visitor pattern. [[doc/win32-callbacks-data.md]]


#ifndef STARTUP_H
#define STARTUP_H

#define WIN32_LEAN_AND_MEAN                     // Exclude rarely used stuff from the SDK
#include <windows.h>                            // SDK MingW

class MainWin
{

// Callbacks are defined static! [[doc/win32-callbacks.md]]
private:
	static INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);		
	
private:
	HINSTANCE			m_hInstance;
	HWND				m_hWnd ;
	const wchar_t*		m_szTitle;
	const wchar_t* 		m_szWindowClass;

	inline void SetHwnd (const  HWND hWnd)						{ m_hWnd = hWnd;}
	inline void SetHinstance( const HINSTANCE& hinst)			{ m_hInstance = hinst; }
	inline void SetTitle( const wchar_t* &szTitle)				{ m_szTitle = szTitle; }
	inline void SetWindowClass(const wchar_t* &szWindowClass)	{ m_szWindowClass = szWindowClass; }

	BOOL InitInstance( int nCmdShow);


public:

	// Tors
	MainWin();
	~MainWin();
	
    // None static! called from: WndProcDispatcher() [[doc/win32-callbacks.md]]	
	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);			
	
	inline HINSTANCE GetInstance() const						{ return m_hInstance;}
	inline HWND GetHwnd() const									{ return m_hWnd; }
	inline const wchar_t*  GetTitle() const						{ return m_szTitle; }
	inline const wchar_t*  GetWindowClass() const				{ return m_szWindowClass; }

	ATOM DefineWindow(HINSTANCE hInstance, WNDPROC WndProc, const wchar_t* szTitle, const wchar_t* szWindowClass);
	bool CreateTheWindow( int nCmdShow, HACCEL outHAccel );
};


#endif