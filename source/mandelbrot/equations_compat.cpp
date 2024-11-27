#include "config.hpp"
#include "equations.hpp"

namespace fractal {
std::complex<complex_underlying>
step(std::complex<complex_underlying> z_n, std::complex<complex_underlying> constant)
{
    return z_n * z_n + constant;
}

std::array<iteration_count, 8> compute_iterations(
    const avx512_complex& z_0, const avx512_complex& constant, iteration_count max_iters
)
{
    std::array<iteration_count, 8> ret{};

    for (uint8_t i = 0; i < 8; i++) {
        iteration_count iterations = 0;
        std::complex<complex_underlying> z_n = {z_0.real[i], z_0.imaginary[i]};
        std::complex<complex_underlying> constant_n = {
            constant.real[i], constant.imaginary[i]
        };
        while (iterations < max_iters && std::norm(z_n) < MANDELBROT_DIVERGENCE_NORM) {
            z_n = step(z_n, constant_n);
            ++iterations;
        }
        ret[i] = iterations;
    }

    return ret;
}
} // namespace fractal
