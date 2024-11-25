#pragma once

#include "units.hpp"

#include <array>

namespace fractal {
struct avx512_complex {
    std::array<complex_underlying, 8> real;
    std::array<complex_underlying, 8> imaginary;
};

} // namespace fractal
