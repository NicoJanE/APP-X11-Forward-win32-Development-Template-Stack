

// C++includes here 

// Application includes
#include "../../../headers/views/_main/WinMain.h"  // self
#include "../../../headers/views/about/AppAbout.h"
#include "../../../headers/views/_main/StartUp.h"			
#include "../../../headers/system/WindowAPI_Def.h"
#include "../../../headers/system/WindowAPI.h"
#include "../../../resource/Resource.h"         // Our resource files



// if using namespace, instead of explicit, then uncomment using namespace WNAPI;			// alias for	==>  namespace WindowNAPI;


MainWin::MainWin()
{}

MainWin::~MainWin()
{}


// private called  CreateTheWindow
BOOL MainWin::InitInstance( int nCmdShow)
{		
	HWND hWnd =WNAPI::CreateWindowExnje(WS_EX_TOPMOST,  GetWindowClass(), GetTitle(), /*WS_POPUPWINDOW*/ WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, /*nullptr*/(HMENU)0, GetInstance(), nullptr);
	if (!hWnd)
	{
		DWORD nErr= GetLastError();
		return FALSE;
	}
   
	SetHwnd(hWnd);
	SetWindowLongPtr(GetHwnd(), GWLP_USERDATA , (LONG_PTR)this);	// Register this object with the hWnd so we can always get it when the hWnd is known, initial it is used in WndProcDispatcher() to dispatch to
																	// to the class/object windows procedure. But it can be used in any, follow up, callback(static) method to get and set the object data
																	// See also the access-or WNAPI::GetObjectFromParentWnd ()
	// Choose one
	//ShowWindow(hWnd, nCmdShow); //AnimateWindow(hWnd,400,AW_SLIDE|AW_HOR_NEGATIVE );	//AnimateWindow(hWnd,800,AW_BLEND );
	AnimateWindow(hWnd,500,AW_CENTER);
	UpdateWindow(hWnd);
	return TRUE;
}


// Public
// 1) Initialize the main Window properties
ATOM MainWin::DefineWindow(HINSTANCE hInstance,  WNDPROC WndProcold, const wchar_t*	 szTitle, const wchar_t* szWindowClass)
{
	SetHinstance(hInstance);
	SetTitle(szTitle);
	SetWindowClass(szWindowClass);

	WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style          = CS_HREDRAW | CS_VREDRAW;			// The CS_ apply for all instances (hence class style). this in contrast with WS_Windows styles which account for each instance, see:CreateWindowExnje )
    wcex.lpfnWndProc    = WndProcold;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32TEMPLATE));					// 1. Make sure to create a own icon resource in the rc file!
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WIN32TEMPLATE);										// 2. Make sure to create a own MENU, ACCELERATORS resource in the rc file!	
    wcex.lpszClassName  = (LPCWSTR )szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}


// Public Create the defined window and display it 
bool MainWin::CreateTheWindow( int nCmdShow, HACCEL outHAccel )
{
	BOOL bResult = InitInstance( nCmdShow);
	outHAccel = LoadAccelerators(GetInstance(), MAKEINTRESOURCE(IDC_WIN32TEMPLATE));				// // 3. Make sure to create a own STRINGTABLE, ID in the rc file!	
	return bResult;
}


// 3) The main entry point for this class/Window, all should go through this
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
LRESULT CALLBACK MainWin::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
				DialogBox(GetInstance(), MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, AppAbout::CB_About);	// Be aware: About() / callbacks must be static or a free function */
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);					// Return to the default
            }
        }
        break;
    case WM_PAINT:
        {
			PAINTSTRUCT ps;
			RECT    rect;

			HDC hdc = BeginPaint(hWnd, &ps);
			GetClientRect(GetHwnd(), &rect);
			DrawText(hdc, L"Hello, Windows", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
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
