// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include <stdlib.h>

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
        char buffer[200];
    case DLL_PROCESS_ATTACH:
    {
    _itoa_s((unsigned int)hModule, buffer, 16);
    OutputDebugStringA("\ndllModule:");
    OutputDebugStringA(buffer);
    HINSTANCE ins = GetModuleHandleA(NULL);
    _itoa_s((unsigned int)ins, buffer, 16);
    OutputDebugStringA("\ng_instance:");
    OutputDebugStringA(buffer);
    }
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

