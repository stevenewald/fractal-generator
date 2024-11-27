#pragma once

#include "units.hpp"

#include <array>

namespace fractal {
struct avx512_complex {
    std::array<complex_underlying, 8> real;
    std::array<complex_underlying, 8> imaginary;
};

struct neon_complex {
    std::array<complex_underlying, 2> real;
    std::array<complex_underlying, 2> imaginary;
};

inline std::array<neon_complex, 4> to_neon_complex(const avx512_complex& complex)
{
    std::array<neon_complex, 4> ret{};
    for (size_t i = 0; i < 4; ++i) {
        ret[i].real[0] = complex.real[i * 2];
        ret[i].imaginary[0] = complex.imaginary[i * 2];
        ret[i].real[1] = complex.real[(i * 2) + 1];
        ret[i].imaginary[1] = complex.imaginary[(i * 2) + 1];
    }
    return ret;
}

} // namespace fractal
