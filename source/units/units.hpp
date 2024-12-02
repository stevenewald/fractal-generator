#pragma once

#include "util.hpp"

#include <cstdint>

namespace fractal {
using iteration_count = std::uint32_t;

using complex_underlying = double;

struct color {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

class Percentage {
    float percentage_;

public:
    constexpr explicit Percentage(float percentage) : percentage_{percentage}
    {
        assert_true(percentage >= 0.0f);
        assert_true(percentage <= 1.0f);
    }

    consteval explicit Percentage() : percentage_{0.0f} {}

    float get_percentage() const { return percentage_; }
};

class Hue {
    float hue_;

public:
    constexpr explicit Hue(float hue) : hue_{hue}
    {
        assert_true(hue >= 0.0f);
        assert_true(hue <= 360.0f);
    }

    float get_hue() const { return hue_; }
};
} // namespace fractal
