#include <windows.h>
#include "resource.h"

typedef INT_PTR (CALLBACK* WndProc)(
	_In_ HWND   hWnd,
	_In_ UINT   message,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
);

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
	HMODULE dllModule = LoadLibraryA("Rvs2Dll.dll");
	WndProc myProc = (WndProc)GetProcAddress(dllModule, "_MyWndProc@16");
	DialogBoxParamA(hInst, MAKEINTRESOURCEA(IDD_DIALOG1), NULL, myProc, 0);
	return 0;
}