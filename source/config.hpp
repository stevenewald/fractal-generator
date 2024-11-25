#pragma once

#include "coordinates.hpp"
#include "units.hpp"

#include <cstddef>

namespace fractal {

constexpr std::size_t WINDOW_WIDTH = 5120;
constexpr std::size_t WINDOW_HEIGHT = 1440;
constexpr std::size_t FRAME_RATE = 60UZ;

constexpr display_domain DISPLAY_DOMAIN{
    {0,                0                },
    {WINDOW_WIDTH - 1, WINDOW_HEIGHT - 1}
};

constexpr complex_domain START_COMPLEX_DOMAIN{
    {complex_underlying{-1.402}, complex_underlying{-.001}},
    {complex_underlying{-1.400}, complex_underlying{.001} }
};

const complex_underlying MANDELBROT_DIVERGENCE_NORM = 4;
const iteration_count MANDELBROT_MAX_ITERATIONS = 1024;
} // namespace fractal
