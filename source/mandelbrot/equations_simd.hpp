#pragma once

#include "config.hpp"
#include "units.hpp"

#include <immintrin.h>

namespace fractal {
// https://en.wikipedia.org/wiki/Mandelbrot_set#Formal_definition

std::complex<complex_underlying>
step(std::complex<complex_underlying> z_n, std::complex<complex_underlying> constant);

std::array<iteration_count, 8> compute_iterations(
    const std::array<std::complex<complex_underlying>, 8>& z_0,
    const std::array<std::complex<complex_underlying>, 8>& constant,
    iteration_count max_iters
);
} // namespace fractal
