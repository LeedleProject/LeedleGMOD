#pragma once

#undef min
#undef max

#include <nicemath.h>


namespace math {
using Vector = nm::float3;
template<size_t size, size_t size2>
using Matrix = nm::mat2x2<float>;
using Quaternion = nm::quat<float>;
using Angle = Vector;
}  // namespace math