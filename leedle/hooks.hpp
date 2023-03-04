#pragma once

#include <memory>
#include <vector>
#include <any>

/*
    This module designed for dymanic hooks (for lua support and etc). Exmaple:
    struct DymanicHook {
        LuaModule lua_module;

        memory::Address<> target = 0;
        memory::Address<> original = 0;


        void initialize() {
            lua_module = get_lua_module();
            target = lua_module.get_target();
        }

        void hook() {
            get_hook_method(lua_module.hook_method())(target, &original, hooked);
        }

        static void hooked() {
            lua_module.call_hook(original);
        }
    };

    If you want to use this module with static hooks (with static methods, lock_cursor for example), you should call hooks::initialize_hooks(DymanicHook{})
    Do not worry about the DymanicHook{} initialization, it will be optimazied by the compiler.
*/

// DESIGEND FOR DYMANIC HOOKS. READ THE DOCUMENTATION IN THIS HEADER FOR MORE INFORMATION.
namespace hooks {
    // If you want to use static hooks, just pass it as an hook_type{}. Value is temperary and will be deleted by compiler on compiletime.
    constexpr auto initialize_hooks(auto&&... hooks) {
        constexpr auto initialize_hook = [](auto&& hook) {
            constexpr auto has_initialize = requires(decltype(hook) _hk) {
                _hk.initialize();
            };

            if constexpr (has_initialize) {
                hook.initialize();
            }
            hook.hook();
        };

        (initialize_hook(hooks), ...);
    }

    constexpr auto remove_hooks(auto&&... hooks) {
        (hooks.unhook(), ...);
    }
}