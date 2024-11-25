#pragma once

/*#include <boost/multiprecision/cpp_bin_float.hpp>
#include <boost/multiprecision/cpp_complex.hpp>
#include <boost/multiprecision/number.hpp>
*/
#include "util.hpp"

#include <complex>
#include <cstdint>

#include <array>

namespace fractal {
using iteration_count = std::uint16_t;

/*using small_float = boost::multiprecision::cpp_bin_float<
    128, boost::multiprecision::digit_base_2, void, int32_t,
    (std::numeric_limits<int32_t>::min() + 1024), 32>;
using complex_underlying = boost::multiprecision::number<small_float>;
using complex = boost::multiprecision::complex_adaptor<small_float>;*/

using complex_underlying = double;

struct avx512_complex {
    std::array<complex_underlying, 8> real;
    std::array<complex_underlying, 8> imaginary;

    std::complex<complex_underlying> get_complex(uint8_t index)
    {
        return {real[index], imaginary[index]};
    }
};

struct color {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

class Ratio {
    float ratio_;

public:
    constexpr explicit Ratio(float ratio) : ratio_{ratio}
    {
        assert_true(ratio_ >= 0.0f);
        assert_true(ratio_ <= 1.0f);
    }

    consteval explicit Ratio() : ratio_{0.0f} {}

    float get_ratio() const { return ratio_; }
};

class Hue {
    float hue_;

public:
    constexpr explicit Hue(float hue) : hue_{hue}
    {
        assert_true(hue >= 0.0f);
        assert_true(hue <= 360.0f);
    }

    float get_hue() const { return hue_; }
};
} // namespace fractal
