#include "hooks.hpp"

#include <algorithm>
#include <functional>
#include <mutex>
#include <ranges>

#include "../game/interfaces/luashared.hpp"


bool game::CreateMoveHook::create_move_hooked(void* self, float time, void* data) {
    static std::once_flag flag;
    std::call_once(flag, [&]() {

    });

    auto result = std::ranges::any_of(
        CREATEMOVE.callbacks | std::views::transform([&](auto& callback) {
            return std::invoke(callback, time, data);
        }) | std::ranges::to<std::vector<bool>>(), [](auto result) { return result; }
    );
    
    return result ? result : CREATEMOVE._hook.original(self, time, data);
}