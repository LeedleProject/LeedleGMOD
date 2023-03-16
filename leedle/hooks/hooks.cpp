#include "hooks.hpp"

#include <algorithm>
#include <functional>
#include <mutex>
#include <ranges>

#include "../game/interfaces/luashared.hpp"


bool game::CreateMoveHook::create_move_hooked(void* self, float time, game::CUserCmd* cmd) {
    static std::once_flag flag;
    std::call_once(flag, [&]() {

    });

    auto result = std::ranges::any_of (
        CREATEMOVE.callbacks | std::views::transform([&](auto& callback) {
            return std::invoke_r<bool>(callback, time, cmd);
        }) | std::ranges::to<std::vector<bool>>(), 
        [](auto result) { return result; }
    );
    
    return result ? result : CREATEMOVE._hook.original(self, time, cmd);
}