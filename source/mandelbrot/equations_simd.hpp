#pragma once

#include "config.hpp"
#include "units.hpp"

namespace fractal {

std::array<iteration_count, 8> compute_iterations(
    const avx512_complex& z_0, const avx512_complex& constant, iteration_count max_iters
);
} // namespace fractal
