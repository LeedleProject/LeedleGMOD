#include <Windows.h>
#include <minwindef.h>
#include <thread>
#include <winuser.h>

#include <imgui.h>

auto entry_point(HMODULE mod) {
    
}

bool __stdcall DllMain(HMODULE mod, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)entry_point, mod, 0, 0);
    }
    return TRUE;
}