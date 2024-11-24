#pragma once

#include <boost/multiprecision/cpp_bin_float.hpp>
#include <boost/multiprecision/cpp_complex.hpp>
#include <boost/multiprecision/number.hpp>

namespace fractal {
using iteration_count = std::uint16_t;

/*using small_float = boost::multiprecision::cpp_bin_float<
    128, boost::multiprecision::digit_base_2, void, int32_t,
    (std::numeric_limits<int32_t>::min() + 1024), 32>;
using complex_underlying = boost::multiprecision::number<small_float>;
using complex = boost::multiprecision::complex_adaptor<small_float>;*/

using complex_underlying = double;

struct color {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};
} // namespace fractal
