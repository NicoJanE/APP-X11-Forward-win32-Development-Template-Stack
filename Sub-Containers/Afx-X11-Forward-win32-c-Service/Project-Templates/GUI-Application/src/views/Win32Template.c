// This file is part of: App-X11-Forward-win32-Development-Template-Stack
// Copyright (c) 2024 Nico Jan Eelhart
// This source code is licensed under the MIT License found in the  'LICENSE.md' file in the root directory of this source tree.

// https://docs.microsoft.com/en-us/windows/desktop/apiindex/windows-api-list

#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include "views/Win32Template.h"

#ifdef _WIN32
#define ENTRY_POINT WinMain
#else
#define ENTRY_POINT wWinMain
#endif




#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE	GLO_hInst;		                              // current instance
WCHAR		GLO_szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR		GLO_szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                WinMainClassDefinition(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


// Entry point
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR    lpCmdLine, _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    
	// TODO: Place code here.



    // 1) Initialize the main Window properties
    LoadStringW(hInstance, IDS_APP_TITLE, GLO_szTitle, MAX_LOADSTRING);				// Set the visible window title name
    
	
//	LoadStringW(hInstance, IDC_WIN32TEMP, GLO_szWindowClass, MAX_LOADSTRING);		// The name of the WNDCLASSEXW structure to identify it in CreateWindowW()
	
	if (!LoadStringW(hInstance, IDC_WIN32TEMP, GLO_szWindowClass, MAX_LOADSTRING)) {
    DWORD dwError = GetLastError();
    wchar_t errorMsg[256];
    swprintf(errorMsg, sizeof(errorMsg) / sizeof(wchar_t), 
             L"Failed to load window class name! Error: %d", dwError);
    MessageBox(NULL, errorMsg, L"Error", MB_OK);
}

	
	
	
	
	
	
	
    WinMainClassDefinition(hInstance);												// Set the properties of the main Window				

    // 2)  Create the Window defined in: WinMainClassDefinition() and display it
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32TEMP));

	// 3) start mesage loop for our Window, call the callback defined in:WinMainClassDefinition (WndProc)
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}


// 1) Initialize the main Window properties, and assign the WndProc() Callback to it
ATOM WinMainClassDefinition(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32TEMP));
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WIN32TEMP);
    wcex.lpszClassName  = GLO_szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}




// 2) Create the Window defined in: WinMainClassDefinition() and display it
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   GLO_hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(GLO_szWindowClass, GLO_szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
   if (!hWnd)
   {
	    DWORD dwError = GetLastError();  // Get the error code
    wchar_t errorMsg[256];
    // Format the error message
    swprintf(errorMsg, sizeof(errorMsg) / sizeof(wchar_t), 
             L"Failed to create window! Error: %d (GLO_szWindowClass: %s)", 
             dwError, 
             GLO_szWindowClass);
    MessageBox(NULL, errorMsg, L"Error", MB_OK);
    return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

// 3) Execute  the callback method defined in WinMainClassDefinition()
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(GLO_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
