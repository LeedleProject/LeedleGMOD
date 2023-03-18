#pragma once

#include <vadefs.h>
#include "hook.hpp"

#include "../game/types/usercmd.hpp"

#include <concepts>
#include <functional>
#include <vector>

namespace game {

struct CreateMoveHook : public hooks::IHook, public WithCallbacks<bool, float, CUserCmd*> {
    memory::hook_methods::MinHook<bool(__fastcall*)(void*, float, game::CUserCmd*)> _hook;

    void initialize() override {
        _hook.detour = create_move_hooked;
    }

    void hook() override {
        auto target = memory::MemoryScanner("40 53 48 83 EC 30 0F 29 74 24 ? 49 8B D8")
            .scan<uintptr_t>(memory::client.begin(), memory::client.end());
        _hook.hook(target.address);

        DLOG_S(INFO) << "CreateMoveHooked: " << std::hex << _hook.original;
    }

    void unhook() override {
        _hook.unhook();
    }

    static bool __fastcall create_move_hooked(void* self, float time, game::CUserCmd* cmd);
};

inline CreateMoveHook CREATEMOVE;

}