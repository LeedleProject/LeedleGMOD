#pragma once

#include "createmove.hpp"

#include "../hooks.hpp"

namespace game {
    constexpr auto initialize_default_hooks() {
        hooks::initialize_hooks(CREATEMOVE);
    }

    constexpr auto shutdown_default_hooks() {
        hooks::remove_hooks(CREATEMOVE);
    }
}