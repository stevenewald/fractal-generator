#pragma once

#include "config.hpp"
#include "units.hpp"

#include <immintrin.h>

namespace fractal {
// https://en.wikipedia.org/wiki/Mandelbrot_set#Formal_definition

inline std::complex<complex_underlying>
step(std::complex<complex_underlying> z_n, std::complex<complex_underlying> constant)
{
    return z_n * z_n + constant;
}

inline std::array<iteration_count, 8> compute_iterations(
    std::array<std::complex<complex_underlying>, 8> z_0,
    std::array<std::complex<complex_underlying>, 8> constant, iteration_count max_iters
)
{
    std::array<iteration_count, 8> res{};
    for (size_t i = 0; i < 8; i++) {
        while (res[i] < max_iters && std::norm(z_0[i]) < MANDELBROT_DIVERGENCE_NORM) {
            z_0[i] = step(z_0[i], constant[i]);
            ++res[i];
        }
    }
	return res;

    std::array<double, 8> reals = {z_0[0].real(), z_0[1].real(), z_0[2].real(),
                                   z_0[3].real(), z_0[4].real(), z_0[5].real(),
                                   z_0[6].real(), z_0[7].real()};
    std::array<double, 8> imags = {z_0[0].imag(), z_0[1].imag(), z_0[2].imag(),
                                   z_0[3].imag(), z_0[4].imag(), z_0[5].imag(),
                                   z_0[6].imag(), z_0[7].imag()};

    std::array<double, 8> squared_reals{};
    std::array<double, 8> squared_imags{};
    std::array<iteration_count, 8> solved_its{0};
    bool some_unsolved = true;
	iteration_count iterations = 0;

    while (some_unsolved && iterations < max_iters) {
        __m512d input_vec_real = _mm512_load_pd(reals.data());
        __m512d squared_vec_real = _mm512_mul_pd(input_vec_real, input_vec_real);
        _mm512_store_pd(squared_reals.data(), squared_vec_real);
        __m512d input_vec_imag = _mm512_load_pd(imags.data());
        __m512d squared_vec_imag = _mm512_mul_pd(input_vec_imag, input_vec_imag);
        _mm512_store_pd(squared_imags.data(), squared_vec_imag);

        for (int i = 0; i < 8; i++) {
            if (solved_its[i] == 0) {
                imags[i] = (2 * reals[i] * imags[i]) + constant[i].imag();
                reals[i] = (squared_reals[i] - squared_imags[i]) + constant[i].real();
            }
        }

        some_unsolved = false;
        for (std::size_t i = 0; i < 8; i++) {
            if (solved_its[i] == 0)
                continue;
            double norm = std::sqrt(reals[i] * reals[i] + imags[i] * imags[i]);
            if (norm < MANDELBROT_DIVERGENCE_NORM)
                solved_its[i] = iterations;
            else
                some_unsolved = true;
        }

        ++iterations;
    }

    return solved_its;
}
} // namespace fractal
