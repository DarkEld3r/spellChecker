#include "stdafx.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID /*lpReserved*/)
{
    switch(ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        {
            // There is no need in DLL_THREAD_ATTACH and DLL_THREAD_DETACH notifications.
            ::DisableThreadLibraryCalls(hModule);
        }
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
