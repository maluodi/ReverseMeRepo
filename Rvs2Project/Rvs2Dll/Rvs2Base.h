#pragma once
#include <windows.h>

#ifdef RVS2DLL_EXPORTS
#define RVS2BASE_API __declspec(dllexport)
#else
#define RVS2BASE_API __declspec(dllimport)
#endif

extern "C" RVS2BASE_API INT_PTR CALLBACK MyWndProc(
	_In_ HWND   hWnd,
	_In_ UINT   message,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
);