#include "equations.hpp"

namespace fractal {
std::array<iteration_count, 8> compute_iterations(
    const std::array<std::complex<complex_underlying>, 8>& z_0,
    const std::array<std::complex<complex_underlying>, 8>& constant,
    iteration_count max_iters
)
{
    static const auto SQUARED_DIVERGENCE =
        MANDELBROT_DIVERGENCE_NORM * MANDELBROT_DIVERGENCE_NORM;

    alignas(64) std::array<double, 8> reals = {z_0[0].real(), z_0[1].real(),
                                               z_0[2].real(), z_0[3].real(),
                                               z_0[4].real(), z_0[5].real(),
                                               z_0[6].real(), z_0[7].real()};
    alignas(64) std::array<double, 8> imags = {z_0[0].imag(), z_0[1].imag(),
                                               z_0[2].imag(), z_0[3].imag(),
                                               z_0[4].imag(), z_0[5].imag(),
                                               z_0[6].imag(), z_0[7].imag()};
    alignas(64) std::array<double, 8> const_reals = {
        constant[0].real(), constant[1].real(), constant[2].real(), constant[3].real(),
        constant[4].real(), constant[5].real(), constant[6].real(), constant[7].real()
    };
    alignas(64) std::array<double, 8> const_imags = {
        constant[0].imag(), constant[1].imag(), constant[2].imag(), constant[3].imag(),
        constant[4].imag(), constant[5].imag(), constant[6].imag(), constant[7].imag()
    };

    std::array<iteration_count, 8> solved_its = {0};

    __m512d input_vec_real = _mm512_load_pd(reals.data());
    __m512d input_vec_imag = _mm512_load_pd(imags.data());
    __m512d input_vec_constant_imags = _mm512_load_pd(const_imags.data());
    __m512d input_vec_constant_reals = _mm512_load_pd(const_reals.data());
    __m512i solved_its_vec = _mm512_loadu_epi16(solved_its.data());

    for (iteration_count iterations = 0; iterations < max_iters; iterations++) {
        // Square real
        __m512d squared_vec_real = _mm512_mul_pd(input_vec_real, input_vec_real);

        // Square imag
        __m512d squared_vec_imag = _mm512_mul_pd(input_vec_imag, input_vec_imag);

        // Create imags
        __m512d real_x2 = _mm512_mul_pd(input_vec_real, _mm512_set1_pd(2));
        input_vec_imag =
            _mm512_fmadd_pd(real_x2, input_vec_imag, input_vec_constant_imags);

        // Create reals
        __m512d subtracted_squared = _mm512_sub_pd(squared_vec_real, squared_vec_imag);
        input_vec_real = _mm512_add_pd(subtracted_squared, input_vec_constant_reals);

        // Create squared norms
        __m512d squared_norms_vec = _mm512_add_pd(squared_vec_real, squared_vec_imag);
        __mmask8 solved_mask = _mm512_cmp_pd_mask(
            squared_norms_vec, _mm512_set1_pd(SQUARED_DIVERGENCE), _CMP_GT_OS
        );

        uint32_t solved = _cvtmask8_u32(solved_mask);
        solved_its_vec = _mm512_mask_blend_epi16(
            solved_mask, solved_its_vec,
            _mm512_set1_epi16(static_cast<int16_t>(iterations))
        );
        if (solved == 0xFF) [[unlikely]]
            break;
    }

    __mmask32 mask = _mm512_cmpeq_epi16_mask(solved_its_vec, _mm512_set1_epi16(0));
    solved_its_vec = _mm512_mask_mov_epi16(
        solved_its_vec, mask, _mm512_set1_epi16(static_cast<int16_t>(max_iters))
    );
    _mm512_storeu_epi16(solved_its.data(), solved_its_vec);

    return solved_its;
}
} // namespace fractal
