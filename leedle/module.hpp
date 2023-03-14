#pragma once

#include <memory>
#include <concepts>
#include <type_traits>

namespace leedle {
    struct Leedle;

    struct IModule {
        virtual ~IModule() = default;
        virtual void setup_hooks() = 0;
        virtual void uninitialize() = 0;
    };

    constexpr auto initialize_modules(auto& ...module) {
        auto initialize_feature = [](auto& mod){
            constexpr auto is_module = requires {
                std::is_base_of_v<IModule, std::remove_reference_t<decltype(mod)>>;
            };

            if constexpr (is_module) {
                mod.setup_hooks();
            }
        };
        (initialize_feature(module), ...);
    }

    constexpr auto shutdown_modules(auto& ...module) {
        auto shutdown_feature = [](auto& mod){
            constexpr auto is_module = requires {
                std::is_base_of_v<IModule, std::remove_reference_t<decltype(mod)>>;
            };

            if constexpr (is_module) {
                mod.uninitialize();
            }
        };
        (shutdown_feature(module), ...);
    }
}