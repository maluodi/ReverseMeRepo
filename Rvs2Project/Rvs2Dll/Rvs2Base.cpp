#include <windows.h>
#include "pch.h"
#include "Rvs2Base.h"
#include <stdlib.h>

const int ID_BUTTON1 = 0x3ED;
const int ID_BUTTON2 = 0x3EE;
const int ID_EDIT1 = 0x3F0;
const int ID_EDIT2 = 0x3EF;

char buffer[200];
HWND g_hWnd;

void setMyText(HWND hWnd, unsigned int editID)
{
	int num = rand();
	num = (num % 100) + 1;
	_itoa_s(num, buffer, 10);
	SetDlgItemTextA(hWnd, editID, buffer);
}

LRESULT CALLBACK MyKeyboardProc( 
	_In_ int    code,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
)
{
	if (code >= 0 && (lParam & 0x80000000)) {
		//lParam:31 bit The transition state. 
		//The value is 0 if the key is being pressed and 1 if it is being released.

		switch (wParam) 
		//wParam The virtual-key code of the key that generated the keystroke message.
		{
		case 0x5A: //Z virtual-key code 
		{
			setMyText(g_hWnd, ID_EDIT1);
			return 1;
		}
		break;
		case 0x4D: //M virtual-key code 
		{
			setMyText(g_hWnd, ID_EDIT2);
			return 1;
		}
		break;
		default:
			break;
		}
	}
	return CallNextHookEx(NULL, code, wParam, lParam);
}

INT_PTR CALLBACK MyWndProc(
	_In_ HWND   hWnd,
	_In_ UINT   message,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam)
{
	switch (message) {
	case WM_INITDIALOG:
	{
		g_hWnd = hWnd;
		HINSTANCE g_instance = GetModuleHandleA(NULL);
		SetWindowsHookEx(WH_KEYBOARD, MyKeyboardProc, g_instance, GetCurrentThreadId());
	}
	break;
	case WM_CLOSE:
		EndDialog(hWnd, 0);
		ExitProcess(0);
		break;
	case WM_DESTROY:
		EndDialog(hWnd, 0);
		ExitProcess(0);
		break;
	case WM_CREATE:
		break;
	case WM_COMMAND:
		if (wParam == ID_BUTTON1) {
			setMyText(hWnd, ID_EDIT1);
		}
		else if (wParam == ID_BUTTON2) {
			setMyText(hWnd, ID_EDIT2);
		}
		break;
	default:
		break;
	}
	return INT_PTR();
}