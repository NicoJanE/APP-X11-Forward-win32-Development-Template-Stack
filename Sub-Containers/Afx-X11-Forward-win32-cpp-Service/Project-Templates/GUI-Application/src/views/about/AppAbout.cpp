
// Application includes
#include "../../../headers/views/_main/WinMain.h" 			
#include "../../../headers/system/WindowAPI.h"
#include "../../../headers/views/about/AppAbout.h"
#include <string>


// Static method message handler for about box. static or free functions are required for WIN32 CALLBACK methods
INT_PTR CALLBACK AppAbout::CB_About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
	MainWin* pMainWin = nullptr;

    switch (message)
    {
    case WM_INITDIALOG:
	{	
        return (INT_PTR)TRUE;
	}
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
			
			// Demostrates how  to acces the parent Window(and change it)
			// Because this is a static method(required WIN32 due to C calling convention for CALLBACK functions\methods)  we must get the original class object again from the
			// correct HWWD so we can use the original data object (MainWin* pMainWin).
			pMainWin = WNAPI::GetObjectFromParentWnd(pMainWin,hDlg);
			WNAPI::ChangeWSStyle(pMainWin->GetHwnd(),   WS_VSCROLL );				
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

