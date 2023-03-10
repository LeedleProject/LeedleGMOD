#pragma once

#include <filesystem>
#include <format>
#include <functional>
#include <platform_folders.h>
#include <string>
#include <string_view>

#include "render.hpp"

#include "memory.hpp"

namespace leedle {
    struct Leedle : leedle::IModule {
        void setup_hooks() override {}
        void uninitialize() override {}

        std::function<void()> unload_function;
    };
    static inline Leedle LEEDLE;

    namespace fs {
        inline auto get_leedle_root() -> std::filesystem::path {
            return sago::getDataHome().append("\\leedlecsgo\\");
        }
    }

    namespace logger {
        inline void initialize_logging();

        template<class ...Args>
        constexpr auto game_message(const std::format_string<Args...> format, Args&& ...args) {
            static memory::MemoryModule mod("tier0.dll");
            static auto conmessage_function = mod.get_symbol<void(__cdecl*)(const char*)>("?ConMsg@@YAXPBDZZ");
            conmessage_function.invoke(std::format(format, args...).append("\n").c_str());
        }

        template<class ...Args>
        constexpr auto game_warning(const std::format_string<Args...> format, Args&& ...args) {
            static memory::MemoryModule mod("tier0.dll");
            static auto warning_function = mod.get_symbol<void(__cdecl*)(const char*)>("Warning");
            warning_function.invoke(std::format(format, args...).append("\n").c_str());
        }
    }
}