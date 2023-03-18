#pragma once

#include <vadefs.h>
#include "hook.hpp"

#include "../game/interfaces/vclient.hpp"

namespace game {

struct FrameStageNotify : public hooks::IHook, public WithCallbacks<void, VClient017*, int> {
    memory::hook_methods::MinHook<void(__fastcall*)(VClient017*, ClientFrameStage)> _hook;

    void initialize() override {
        _hook.detour = frame_stage_notify_hooked;
    }

    void hook() override {
        _hook.hook(memory::MemoryScanner("48 83 EC 28 89 15 ? ? ? ?").scan<uintptr_t>(memory::client).address);
        DLOG_S(INFO) << "FrameStageNotifyHooked: " << std::hex << _hook.original;
    }

    void unhook() override {
        _hook.unhook();
    }

    static void __fastcall frame_stage_notify_hooked(VClient017* self, ClientFrameStage stage);
};

inline FrameStageNotify FRAMESTAGENOTIFY;

}