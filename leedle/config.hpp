#pragma once

#include <filesystem>
#include <string_view>
#include <toml.hpp>

#include "leedle.hpp"
#include "traits.hpp"

namespace leedle {

namespace cfg {

TRAIT_STRUCT(Configuratable,
    TRAIT_METHOD(void, save_variables),
    TRAIT_METHOD(void, load_variavles)
)

template<class Type, const char* TemplatedName>
class ConfigVariable {
    std::string name;
    Type value;

public:
    constexpr ConfigVariable(Type&& default_value) : name(TemplatedName), value(std::forward<Type>(default_value)) {}
};

inline auto create_config_file(std::string_view name) {
    auto path = fs::get_leedle_root() / "config";
    if (!std::filesystem::exists(path)) {
        std::filesystem::create_directories(path);
    }
    path /= name;
    fs::create_file(path.string());
    return path;
}

}

}