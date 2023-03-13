#pragma once

#include <vadefs.h>
#include "hook.hpp"

#include <concepts>
#include <functional>
#include <vector>

namespace game {

struct CreateMoveHook : public hooks::IHook {
    using CreateMoveCallbackFn = bool(float, void*);

    memory::hook_methods::MinHook<bool(__fastcall*)(void*, float, void*)> _hook;

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

    std::vector<std::function<CreateMoveCallbackFn>> callbacks;

    inline auto add_callback(const std::invocable<bool, float, void*> auto& callback) {
        callbacks.emplace_back(std::move(callback));

        DLOG_S(INFO) << "Added callback: " << std::hex << std::hash<decltype(callback)>{}(callback);
    }

    static bool __fastcall create_move_hooked(void* self, float time, void* data);
};

inline CreateMoveHook CREATEMOVE;

}