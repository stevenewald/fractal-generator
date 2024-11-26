#include "config.hpp"
#include "equations.hpp"
#include "units/units.hpp"
#include "units/units_avx.hpp"

#include <arm_neon.h>

namespace fractal {
std::array<iteration_count, 2> compute_iterations_neon(
    const neon_complex& z_0, const neon_complex& constant, iteration_count max_iters
)
{
    static const auto SQUARED_DIVERGENCE =
        MANDELBROT_DIVERGENCE_NORM * MANDELBROT_DIVERGENCE_NORM;

    alignas(16) std::array<double, 2> reals = z_0.real;
    alignas(16) std::array<double, 2> imags = z_0.imaginary;
    alignas(16) std::array<double, 2> const_reals = constant.real;
    alignas(16) std::array<double, 2> const_imags = constant.imaginary;

    float64x2_t input_vec_real = vld1q_f64(reals.data());
    float64x2_t input_vec_imag = vld1q_f64(imags.data());
    float64x2_t input_vec_constant_reals = vld1q_f64(const_reals.data());
    float64x2_t input_vec_constant_imags = vld1q_f64(const_imags.data());

    uint64x2_t solved_its_vec = vdupq_n_u64(0);
    float64x2_t squared_divergence_vec = vdupq_n_f64(SQUARED_DIVERGENCE);
    uint64x2_t active_mask = vdupq_n_u64(~0ULL); // all bits set

    for (uint64_t iterations = 0; iterations < max_iters; iterations++) {
        // load current values
        float64x2_t x = input_vec_real;
        float64x2_t y = input_vec_imag;

        // compute squares and product
        float64x2_t x_squared = vmulq_f64(x, x);
        float64x2_t y_squared = vmulq_f64(y, y);
        float64x2_t xy = vmulq_f64(x, y);

        // Update real part: input_vec_real = x_squared - y_squared + constant_reals
        float64x2_t temp_real = vsubq_f64(x_squared, y_squared);
        input_vec_real = vaddq_f64(temp_real, input_vec_constant_reals);

        // update imaginary part: input_vec_imag = 2 * xy + constant_imags
        input_vec_imag = vmlaq_f64(input_vec_constant_imags, xy, vdupq_n_f64(2.0));

        // compute squared norms
        float64x2_t squared_norms_vec = vaddq_f64(x_squared, y_squared);

        // determine which elements have diverged
        uint64x2_t solved_mask = vcgtq_f64(squared_norms_vec, squared_divergence_vec);

        // update iteration counts for elements that have just diverged
        uint64x2_t iteration_vec = vdupq_n_u64(iterations);
        solved_its_vec = vbslq_u64(solved_mask, iteration_vec, solved_its_vec);

        uint64x2_t not_solved_mask =
            veorq_u64(solved_mask, vdupq_n_u64(~0ULL)); // Compute bitwise NOT
        active_mask = vandq_u64(active_mask, not_solved_mask);

        // Reduce active_mask to check if all lanes are zero
        if (vaddvq_u64(active_mask) == 0) [[unlikely]]
            break;
    }

    // // set iteration counts to max_iters where they haven't diverged
    uint64x2_t zero_vec = vdupq_n_u64(0);
    uint64x2_t zero_mask = vceqq_u64(solved_its_vec, zero_vec);
    int64x2_t max_iters_vec = vdupq_n_u64(static_cast<uint64_t>(max_iters));
    solved_its_vec = vbslq_u64(zero_mask, max_iters_vec, solved_its_vec);

    // store the iteration counts
    alignas(16) std::array<uint64_t, 2> ret{};
    vst1q_u64(ret.data(), solved_its_vec);
    std::array ret2{static_cast<uint16_t>(ret[0]), static_cast<uint16_t>(ret[1])};

    return ret2;
}

std::array<iteration_count, 8> compute_iterations(
    const avx512_complex& z_0, const avx512_complex& constant, iteration_count max_iters
)
{
    std::array<iteration_count, 8> ret{};
    auto neons_z0 = to_neon_complex(z_0);
    auto neons_const = to_neon_complex(constant);
    for (uint8_t i = 0; i < 4; ++i) {
        auto [it_count_1, it_count_2] =
            compute_iterations_neon(neons_z0[i], neons_const[i], max_iters);
        ret[i * 2] = it_count_1;
        ret[i * 2 + 1] = it_count_2;
    }
    return ret;
}
} // namespace fractal
