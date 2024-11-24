#pragma once

#include "config.hpp"
#include "units.hpp"

namespace fractal {
// https://en.wikipedia.org/wiki/Mandelbrot_set#Formal_definition
inline std::complex<complex_underlying>
step(std::complex<complex_underlying> z_n, std::complex<complex_underlying> constant)
{
    return z_n * z_n + constant;
}

inline iteration_count compute_iterations(
    std::complex<complex_underlying> z_0, std::complex<complex_underlying> constant,
    iteration_count max_iters
)
{
    iteration_count iterations = 0;
    std::complex<complex_underlying> z_n = z_0;

    while (iterations < max_iters && std::norm(z_n) < MANDELBROT_DIVERGENCE_NORM) {
        z_n = step(z_n, constant);
        ++iterations;
    }

    return iterations;
}
} // namespace fractal
