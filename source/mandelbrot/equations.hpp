#pragma once

#include "units.hpp"

namespace fractal {
// https://en.wikipedia.org/wiki/Mandelbrot_set#Formal_definition
std::complex<complex_underlying>
step(std::complex<complex_underlying> z_n, std::complex<complex_underlying> constant);

iteration_count compute_iterations(
    std::complex<complex_underlying> z_0, std::complex<complex_underlying> constant,
    iteration_count max_iters
);
} // namespace fractal
