#pragma once

#include "module.hpp"

namespace features {

struct Features : public leedle::IModule {
    void setup_hooks() override {}
    void uninitialize() override {}
};

inline Features FEATURES;

}