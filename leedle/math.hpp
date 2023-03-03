#pragma once

#undef min
#undef max

#include <psst/math/vector.hpp>
#include <psst/math/matrix.hpp>
#include <psst/math/angles.hpp>
#include <psst/math/matrix.hpp>
#include <psst/math/quaternion.hpp>

namespace math {
    using Vector = psst::math::vector<float, 3>;
    using VectorView = psst::math::vector_view<float, 3>;
    template<size_t size, size_t size2> 
        using Matrix = psst::math::matrix<float, size, size2>;
    using Quaternion = psst::math::quaternion<float>;
}