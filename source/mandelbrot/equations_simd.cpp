#include "config.hpp"
#include "equations.hpp"
#include "units/units.hpp"
#include "units/units_avx.hpp"

#include <immintrin.h>

namespace fractal {
std::array<iteration_count, 8> compute_iterations(
    const avx512_complex& z_0, const avx512_complex& constant, iteration_count max_iters
)
{
    static const auto SQUARED_DIVERGENCE =
        MANDELBROT_DIVERGENCE_NORM * MANDELBROT_DIVERGENCE_NORM;

    alignas(64) std::array<double, 8> reals = z_0.real;
    alignas(64) std::array<double, 8> imags = z_0.imaginary;
    alignas(64) std::array<double, 8> const_reals = constant.real;
    alignas(64) std::array<double, 8> const_imags = constant.imaginary;

    __m512d input_vec_real = _mm512_load_pd(reals.data());
    __m512d input_vec_imag = _mm512_load_pd(imags.data());
    __m512d input_vec_constant_imags = _mm512_load_pd(const_imags.data());
    __m512d input_vec_constant_reals = _mm512_load_pd(const_reals.data());

    __m256i solved_its_vec = _mm256_set1_epi32(0);
    const __m512d squared_divergence_vec = _mm512_set1_pd(SQUARED_DIVERGENCE);
    __mmask8 active_mask = 0xFF;

    for (iteration_count iterations = 0; iterations < max_iters; iterations++) {
        // compute squares and product
        __m512d x_squared = _mm512_mul_pd(input_vec_real, input_vec_real);
        __m512d y_squared = _mm512_mul_pd(input_vec_imag, input_vec_imag);
        __m512d xy = _mm512_mul_pd(input_vec_real, input_vec_imag);

        // update real part: input_vec_real = x_squared - y_squared + constant_reals
        input_vec_real = _mm512_add_pd(
            _mm512_sub_pd(x_squared, y_squared), input_vec_constant_reals
        );

        // update imaginary part: input_vec_imag = 2 * xy + constant_imags
        input_vec_imag =
            _mm512_fmadd_pd(xy, _mm512_set1_pd(2.0), input_vec_constant_imags);

        // compute squared norms
        __m512d squared_norms_vec = _mm512_add_pd(x_squared, y_squared);

        // determine which elements have diverged
        active_mask =
            _mm512_cmp_pd_mask(squared_norms_vec, squared_divergence_vec, _CMP_LE_OS);

        // update iteration counts for elements that have just diverged
        solved_its_vec = _mm256_mask_blend_epi32(
            active_mask, solved_its_vec, _mm256_set1_epi32(static_cast<int>(iterations))
        );

        // break if all elements have diverged
        if (active_mask == 0) [[unlikely]]
            break;
    }

    __mmask8 mask = _mm256_cmpeq_epi32_mask(
        solved_its_vec, _mm256_set1_epi32(static_cast<int>(max_iters) - 1)
    );
    solved_its_vec = _mm256_mask_mov_epi32(
        solved_its_vec, mask, _mm256_set1_epi32(static_cast<int16_t>(max_iters))
    );

    alignas(32) std::array<iteration_count, 8> ret{};
    _mm256_storeu_epi32(ret.data(), solved_its_vec);

    return ret;
}
} // namespace fractal
