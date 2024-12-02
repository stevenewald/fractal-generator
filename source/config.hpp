#pragma once

#include "units/coordinates.hpp"
#include "units/units.hpp"

#include <cstddef>

namespace fractal {

constexpr std::size_t WINDOW_WIDTH = 800UZ * 2;
constexpr std::size_t WINDOW_HEIGHT = 600UZ * 2;
constexpr std::size_t FRAME_RATE = 60UZ;

constexpr complex_domain START_COMPLEX_DOMAIN{
    complex_coordinate{-2, -1},
    complex_coordinate{1,  1.25 }
};

const complex_underlying MANDELBROT_DIVERGENCE_NORM = 4;
const iteration_count MANDELBROT_MAX_ITERATIONS = 1024;
} // namespace fractal
