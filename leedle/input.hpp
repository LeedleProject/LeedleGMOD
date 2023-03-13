#pragma once

#include <combaseapi.h>
#include <vadefs.h>
#include "module.hpp"
#include "hooks.hpp"

#include "game/interfaces/interface.hpp"
#include "memory.hpp"

#include <loguru.hpp>

namespace input {
    struct LockCursorHook: public hooks::IHook {
        using function_type = void(__fastcall*)(void*);

        memory::hook_methods::MinHook<function_type> _hook;

        static void __stdcall lock_cursor_hooked(void* data);

        void initialize() override {}
        void hook() override {
            auto mod = game::Interface<uintptr_t>::create_interface
                (memory::material_surface, "VGUI_Surface030");
            DLOG_S(INFO) << "VGUI_Surface030: " << std::hex << mod._self;
            _hook.detour = lock_cursor_hooked;
            _hook.hook(mod.get_virtual_table()[66]);
        }
        void unhook() override {
            _hook.unhook();
        }
    };

    class Input: public leedle::IModule {
        bool should_lock_cursor = true;
    public:
        LockCursorHook lock_cursor_hook;

        virtual void setup_hooks() override;
        virtual void uninitialize() override;

        auto is_cursor_locked() { return should_lock_cursor; }
        auto lock_cursor() { should_lock_cursor = true; }
        auto unlock_cursor() { should_lock_cursor = false; }
    };

    inline Input INPUT;
}