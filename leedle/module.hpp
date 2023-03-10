#pragma once

#include <memory>
namespace leedle {
    struct Leedle;

    struct IModule {
        virtual ~IModule() = default;
        virtual void setup_hooks() = 0;
        virtual void uninitialize() = 0;
    };
}