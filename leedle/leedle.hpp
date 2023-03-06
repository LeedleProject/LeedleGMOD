#pragma once

namespace leedle {
    struct IModule {
        virtual ~IModule() = default;
        virtual void setup_hooks() = 0;
        virtual void uninitialize() = 0;
    };
}