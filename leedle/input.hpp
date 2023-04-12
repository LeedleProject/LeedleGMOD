#pragma once

#include <combaseapi.h>
#include <vadefs.h>

#include <loguru.hpp>

#include "game/interfaces/interface.hpp"
#include "hooks.hpp"
#include "memory.hpp"
#include "module.hpp"


namespace input {
    
struct LockCursorHook: public hooks::IHook {
    using function_type = void(__fastcall*)(void*);

    memory::hook_methods::MinHook<function_type> _hook;

    static void __stdcall lock_cursor_hooked(void* data);

    void initialize() override {}
    void hook() override {
        auto mod = game::Interface<uintptr_t>::create_interface(
            memory::material_surface,
            "VGUI_Surface030");
        DLOG_S(INFO) << "VGUI_Surface030: " << std::hex << mod._self;
        _hook.detour = lock_cursor_hooked;
        _hook.hook(mod.get_virtual_table()[66]);
    }
    void unhook() override {
        _hook.unhook();
    }
};

class Input : public Singleton<Input> {
    bool should_lock_cursor = true;

  public:
    LockCursorHook lock_cursor_hook;

    void initialize();
    void shutdown();

    auto is_cursor_locked() {
        return should_lock_cursor;
    }
    auto lock_cursor() {
        should_lock_cursor = true;
    }
    auto unlock_cursor() {
        should_lock_cursor = false;
    }
};

}  // namespace input