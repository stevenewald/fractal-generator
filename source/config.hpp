#pragma once

#include "coordinates.hpp"

#include <cstddef>

namespace fractal {

constexpr std::size_t WINDOW_WIDTH = 800UZ;
constexpr std::size_t WINDOW_HEIGHT = 600UZ;
constexpr std::size_t FRAME_RATE = 60UZ;

constexpr display_domain DISPLAY_DOMAIN{
    {0,                0                },
    {WINDOW_WIDTH - 1, WINDOW_HEIGHT - 1}
};

constexpr complex_domain START_COMPLEX_DOMAIN{
    {complex_underlying{-2}, complex_underlying{-1.5}},
    {complex_underlying{1},  complex_underlying{1.5} }
};
} // namespace fractal
