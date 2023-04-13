#pragma onc

#include "feature.hpp"
#include "../singleton.hpp"

namespace features {

struct BunnyHop : public Singleton<BunnyHop> {
    void initialize(){}
    void shutdown(){}
};

}