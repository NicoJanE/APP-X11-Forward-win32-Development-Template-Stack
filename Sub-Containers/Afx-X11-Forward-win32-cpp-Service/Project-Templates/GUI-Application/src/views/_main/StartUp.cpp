// Win32Temp.cpp : Defines the entry point for the application.
// https://docs.microsoft.com/en-us/windows/desktop/apiindex/windows-api-list

// We will use UNICODE & STANDARD C types: DEBUG
// see: [[doc/conventions/WindowsTypes-CTypes.md]]

 

// System include
#include <windows.h>

// C++includes here


// Application includes here
#include "../../../headers/views/_main/WinMain.h"
#include "../../../headers/views/_main/StartUp.h"			// Self Startup
#include "../../../headers/system/WindowAPI_Def.h"
#include "../../../resource/Resource.h"


// Consts
#define MAX_LOADSTRING 100


// Global Variables:
HINSTANCE	GLO_hInst;		                              // current instance
wchar_t		GLO_szTitle[MAX_LOADSTRING];                  // The title bar text
wchar_t		GLO_szWindowClass[MAX_LOADSTRING];            // the main window class name


// Free function
// Static application Window Procedure. This dispatcher which will dispatch message for this application to the correct Window procedure of (WndProc) the class/object
//
// This dispatcher requires that the object of the class is set after the creation of the window in the class itself (see InitInstance m method of the class)
// by execution the statement: SetWindowLongPtr(GetHwnd(), GWLP_USERDATA, (LONG_PTR)this);
// This will set the class in the GWLP_USERDATA data member of the object.
//
// In this static window procedure dispatcher we will look for that object in the member (GWLP_USERDATA) if found we will execute the window procedure from 
// the object itself and forward the parameters of this method(hWnd,message, wParam, lParam) If we don't find any object to dispatch to we will call the and return to
// the default Windows procedure(DefWindowProc)
LRESULT static CALLBACK WndProcDispatcher(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// Always make sure to connect to the correct Window!	
	MainWin *pMainWin;	
	pMainWin = reinterpret_cast<MainWin*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	if(pMainWin)	
		return pMainWin->WndProc( hWnd,  message,  wParam,  lParam);
	
	// Other object test here
	else if(pMainWin == nullptr)	
		return DefWindowProc(hWnd, message, wParam, lParam);
	
	return DefWindowProc(hWnd, message, wParam, lParam);
}



// Entry point function       
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ wchar_t*    lpCmdLine, _In_ int nCmdShow)
{
	MainWin mainWin;																	// Object-oriented shift	
	HACCEL hAccelTable = nullptr;
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	// Main window
	// 0) Identifiers for the Window
	LoadStringW(hInstance, IDS_APP_TITLE, GLO_szTitle, MAX_LOADSTRING);					// Set the visible window title name
	LoadStringW(hInstance, IDC_WIN32TEMPLATE, GLO_szWindowClass, MAX_LOADSTRING);		// The name of the WNDCLASSEXW structure  to  identify it in CreateWindowW()

    // 1) Initialize the main Window properties
	mainWin.DefineWindow(hInstance,WndProcDispatcher, GLO_szTitle, GLO_szWindowClass);

    // 2)  Create the Window defined in: WinMainClassDefinition() and display it
	if ( !mainWin.CreateTheWindow( nCmdShow, hAccelTable) )
		return false;
	
	// Optional define other main windows here like above, see also WndProcDispatcher() !!


	// 3) start global application message loop. If there is a message for our application, the callback WndProcDispatcher (WndProc) will be called, to rout the message to the correct object
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
	return (int)msg.wParam;
}



