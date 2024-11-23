#pragma once

#include "coordinates.hpp"

#include <complex>

namespace fractal {
class DisplayToComplexCoordinates {
    float real_scaling_factor_;
    float imaginary_scaling_factor_;
    complex_coordinate complex_domain_start_;

    static float real_scaling_factor(
        const display_coordinate& display_top_right,
        const complex_domain& complex_domain
    )
    {
        float real_d_size = real_domain_size(complex_domain);
        return real_d_size / static_cast<float>(display_top_right.first);
    }

    static float imaginary_scaling_factor(
        const display_coordinate& display_top_right,
        const complex_domain& complex_domain
    )
    {
        float imaginary_d_size = imaginary_domain_size(complex_domain);
        return imaginary_d_size / static_cast<float>(display_top_right.second);
    }

    static complex_coordinate to_complex(display_coordinate coordinate)
    {
        return {
            static_cast<float>(coordinate.first), static_cast<float>(coordinate.second)
        };
    }

public:
    DisplayToComplexCoordinates(
        display_coordinate display_top_right, const complex_domain& complex_domain
    ) :
        real_scaling_factor_(real_scaling_factor(display_top_right, complex_domain)),
        imaginary_scaling_factor_(
            imaginary_scaling_factor(display_top_right, complex_domain)
        ),
        complex_domain_start_(complex_domain.start_coordinate)
    {}

    complex_coordinate to_complex_projection(display_coordinate display_coord)
    {
        std::complex<float> raw_complex_coord = to_complex(display_coord);
        std::complex offset = {
            real_scaling_factor_ * raw_complex_coord.real(),
            imaginary_scaling_factor_ * raw_complex_coord.imag()
        };
        return complex_domain_start_ + offset;
    }
};
} // namespace fractal
