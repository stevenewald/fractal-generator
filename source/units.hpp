#pragma once

#include <boost/multiprecision/cpp_bin_float.hpp>
#include <boost/multiprecision/cpp_complex.hpp>
#include <boost/multiprecision/fwd.hpp>
#include <boost/multiprecision/number.hpp>

#include <limits>

namespace fractal {
using small_float = boost::multiprecision::cpp_bin_float<
    128, boost::multiprecision::digit_base_2, void, int32_t,
    (std::numeric_limits<int32_t>::min() + 1024), 32>;
// using complex_underlying = boost::multiprecision::number<small_float>;
// using complex = boost::multiprecision::complex_adaptor<small_float>;
using complex_underlying = double;
} // namespace fractal
