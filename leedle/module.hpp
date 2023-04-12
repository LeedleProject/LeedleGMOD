#pragma once

#include <memory>
#include <concepts>
#include <type_traits>

#include "traits.hpp"
#include "singleton.hpp"

namespace leedle {
    struct Leedle;

    TRAIT_STRUCT(ModuleTrait, 
        TRAIT_METHOD(void, initialize),
        TRAIT_METHOD(void, shutdown)
    )

    constexpr auto initialize_modules(auto& ...module) {
        auto initialize_module = [](ModuleTrait mod){
            mod.initialize();
        };
        (initialize_module(module), ...);
    }

    constexpr auto shutdown_modules(auto& ...module) {
        auto shutdown_module = [](ModuleTrait mod){
            mod.shutdown();
        };
        (shutdown_module(module), ...);
    }
}