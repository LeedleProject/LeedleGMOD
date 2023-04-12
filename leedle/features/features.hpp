#pragma once

#include "../module.hpp"
#include "feature.hpp"

#include <algorithm>
#include <memory>
#include <vector>

#include "bhop.hpp"

namespace features {

class Features : public Singleton<Features> {
    std::vector<std::shared_ptr<IFeature>> features;
public:
    void initialize() {
        // features = {
        //     std::make_shared<BunnyHop>(),
        // };

        std::ranges::for_each(features, [](auto& feature) {
            feature->setup_hooks();
        });
    }

    void shutdown() {
        std::ranges::for_each(features, [](auto& feature) {
            feature->uninitialize();
        });
    }
};

}