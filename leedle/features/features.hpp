#pragma once

#include "../module.hpp"
#include "feature.hpp"

#include <algorithm>
#include <memory>
#include <vector>

#include "bhop.hpp"

namespace features {

class Features : public Singleton<Features> {
    constexpr auto initialize_features(auto& ...features) {
        constexpr auto initializf = [](Feature ft) {
            ft.initialize();
        };
        (initializf(features), ...);
    }
    constexpr auto shutdown_features(auto& ...features) {
        constexpr auto shutdownf = [](Feature ft) {
            ft.shutdown();
        };
        (shutdownf(features), ...);
    }
public:
    void initialize() {
        initialize_features(BunnyHop::get());
    }

    void shutdown() {
        shutdown_features(BunnyHop::get());
    }
};

}