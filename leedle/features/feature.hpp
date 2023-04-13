#pragma once

#include <traits.hpp>

namespace features {

TRAIT_STRUCT(Feature, 
    TRAIT_METHOD(void, initialize),
    TRAIT_METHOD(void, shutdown)
)

}

