#include "hooks.hpp"

#include <algorithm>
#include <functional>
#include <mutex>
#include <ranges>

#include "../game/interfaces/luashared.hpp"
#include "../game/types/baseentity.hpp"


bool game::CreateMoveHook::create_move_hooked(void* self, float time, game::CUserCmd* cmd) {
    static std::once_flag flag;
    std::call_once(flag, [&]() {

    });

    if (cmd != nullptr && cmd->command_number > 0) {
        auto* local_player = Player::get_local_player();
        if (local_player != nullptr) {
            static bool should_fake = false;
            if (static bool last_jumped = false; !last_jumped && should_fake) {
                should_fake = false;
                cmd->buttons |= IN_JUMP;
            } else if (cmd->buttons & IN_JUMP) {
                if (local_player->flags() & (1 << 0)) {
                    last_jumped = true;
                    should_fake = true;
                } else {
                    cmd->buttons &= ~IN_JUMP;
                    last_jumped = false;
                }
            } else {
                last_jumped = false;
                should_fake = false;
            }
        }
    }

    auto result = std::ranges::any_of (
        CREATEMOVE.callbacks | std::views::transform([&](auto& callback) {
            return std::invoke_r<bool>(callback, time, cmd);
        }) | std::ranges::to<std::vector<bool>>(), 
        [](auto result) { return result; }
    );
    
    return result ? result : CREATEMOVE._hook.original(self, time, cmd);
}

void __fastcall game::FrameStageNotify::frame_stage_notify_hooked(VClient017* self, ClientFrameStage stage) {
    return game::FRAMESTAGENOTIFY._hook.original(self, stage);
}