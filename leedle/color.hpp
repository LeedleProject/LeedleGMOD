#pragma once

#include <array>
#include <stdint.h>

struct Color {
    std::array<uint8_t, 4> rgba;

    Color() = default;
    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : rgba{ r, g, b, a } {}
    Color(uint8_t r, uint8_t g, uint8_t b) : rgba{ r, g, b, 255 } {}

    // copy constructor
    Color(const Color& other) = default;
    // move constructor
    Color(Color&& other) = default;

    // copy assignment
    Color& operator=(const Color& other) = default;
    // move assignment
    Color& operator=(Color&& other) = default;

    // comparison operators
    bool operator==(const Color& other) const {
        return rgba == other.rgba;
    }
    bool operator!=(const Color& other) const {
        return !(*this == other);
    }

    auto& r() { return rgba[0]; }
    auto& g() { return rgba[1]; }
    auto& b() { return rgba[2]; }
    auto& a() { return rgba[3]; }
};

namespace colors {
    inline auto BLACK = Color(0, 0, 0, 255);
    inline auto WHITE = Color(255, 255, 255, 255);
    inline auto RED = Color(255, 0, 0, 255);
    inline auto GREEN = Color(0, 255, 0, 255);
    inline auto BLUE = Color(0, 0, 255, 255);
    inline auto YELLOW = Color(255, 255, 0, 255);
    inline auto MAGENTA = Color(255, 0, 255, 255);
    inline auto CYAN = Color(0, 255, 255, 255);
    inline auto GRAY = Color(128, 128, 128, 255);
}