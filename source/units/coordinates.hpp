#pragma once

#include "units/units.hpp"
#include "util.hpp"

#include <complex>
#include <concepts>

namespace fractal {

template <typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;

struct display_coordinate {
    using underlying = uint16_t;

    underlying x;
    underlying y;

    display_coordinate(underlying x_pos, underlying y_pos) : x{x_pos}, y{y_pos} {}

    template <Numeric T>
    display_coordinate(T x_pos, T y_pos) :
        x{static_cast<underlying>(x_pos)}, y{static_cast<underlying>(y_pos)}
    {
        assert_true(x_pos <= std::numeric_limits<underlying>::max());
        assert_true(y_pos <= std::numeric_limits<underlying>::max());
        assert_true(std::numeric_limits<underlying>::min() <= x_pos);
        assert_true(std::numeric_limits<underlying>::min() <= y_pos);
    }
};

using complex_coordinate = std::complex<complex_underlying>;

uint32_t
decay_2d_coordinate(const display_coordinate& coordinate, uint32_t display_width);

display_coordinate generate_1d_coordinate(uint32_t coordinate, uint32_t display_width);

struct complex_domain {
    complex_coordinate start_coordinate;
    complex_coordinate end_coordinate;
};

inline complex_underlying real_domain_size(const complex_domain& domain)
{
    return domain.end_coordinate.real() - domain.start_coordinate.real();
}

inline complex_underlying imaginary_domain_size(const complex_domain& domain)
{
    return domain.end_coordinate.imag() - domain.start_coordinate.imag();
}

} // namespace fractal
