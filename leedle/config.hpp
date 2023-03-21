#pragma once

#include <string_view>
namespace leedle {

namespace cfg {

struct IConfiguratable {
    virtual void save_variables() = 0;
    virtual void load_variables() = 0;
};

template<class Type, const char* TemplatedName>
class ConfigVariable {
    std::string name;
    Type value;

public:
    constexpr ConfigVariable(Type&& default_value) : name(TemplatedName), value(std::forward<Type>(default_value)) {}

    
};

}

}