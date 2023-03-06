#include <Windows.h>
#include <debugapi.h>
#include <functional>
#include <minwindef.h>
#include <thread>
#include <type_traits>
#include <winuser.h>

#include "math.hpp"

#include "memory.hpp"
#include "hooks.hpp"

#include "render.hpp"

auto warning(std::string_view message) {
    using warning_fn = void(__cdecl*)(const char*);
    static memory::MemoryModule mod("tier0.dll");
    static auto warning_function = mod.get_symbol<warning_fn>("Warning");

    warning_function.invoke(message.data());
}

struct basic_hook {
    static void initialize() {
        warning("Hello from hook!\n");
    }

    static void hook() {
        warning("Hello from hook!x2\n");
    }

    static void unhook() {
        warning("Hello from hook!x3\n");
    }
};

auto __stdcall entry_point(HMODULE mod) {
    render::Render render;
    render.setup_hooks();
}

bool __stdcall DllMain(HMODULE mod, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)entry_point, mod, 0, 0);
    }
    return TRUE;
}