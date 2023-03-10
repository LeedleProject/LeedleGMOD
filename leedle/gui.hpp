#pragma once

#include "module.hpp"

namespace gui {
    
struct GUI: public leedle::IModule {
    GUI() {}

    void setup_hooks() override;
    void uninitialize() override;
};

static inline GUI GUI;
}  // namespace gui