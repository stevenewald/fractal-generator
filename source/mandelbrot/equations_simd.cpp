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

    __m128i solved_its_vec = _mm_set1_epi16(0);
    const __m512d squared_divergence_vec = _mm512_set1_pd(SQUARED_DIVERGENCE);
    __mmask8 active_mask = 0xFF;

    for (iteration_count iterations = 0; iterations < max_iters; iterations++) {
        // load current values
        __m512d x = input_vec_real;
        __m512d y = input_vec_imag;

        // compute squares and product
        __m512d x_squared = _mm512_mul_pd(x, x);
        __m512d y_squared = _mm512_mul_pd(y, y);
        __m512d xy = _mm512_mul_pd(x, y);

        // update real part: input_vec_real = x_squared - y_squared + constant_reals
        __m512d temp_real = _mm512_sub_pd(x_squared, y_squared);
        input_vec_real = _mm512_add_pd(temp_real, input_vec_constant_reals);

        // update imaginary part: input_vec_imag = 2 * xy + constant_imags
        input_vec_imag =
            _mm512_fmadd_pd(xy, _mm512_set1_pd(2.0), input_vec_constant_imags);

        // compute squared norms
        __m512d squared_norms_vec = _mm512_add_pd(x_squared, y_squared);

        // determine which elements have diverged
        __mmask8 solved_mask =
            _mm512_cmp_pd_mask(squared_norms_vec, squared_divergence_vec, _CMP_GT_OS);

        // update iteration counts for elements that have just diverged
        solved_its_vec = _mm_mask_blend_epi16(
            solved_mask, solved_its_vec,
            _mm_set1_epi16(static_cast<int16_t>(iterations))
        );

        // update active mask to skip computations for diverged elements
        active_mask = _kandn_mask8(solved_mask, active_mask);

        // break if all elements have diverged
        if (active_mask == 0) [[unlikely]]
            break;
    }

    __mmask8 mask = _mm_cmpeq_epi16_mask(solved_its_vec, _mm_set1_epi16(0));
    solved_its_vec = _mm_mask_mov_epi16(
        solved_its_vec, mask, _mm_set1_epi16(static_cast<int16_t>(max_iters))
    );

    alignas(16) std::array<iteration_count, 8> ret{};
    _mm_storeu_epi16(ret.data(), solved_its_vec);

    return ret;
}
} // namespace fractal
