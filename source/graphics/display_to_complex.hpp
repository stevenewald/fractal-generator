#pragma once

#include "units/display_domain.hpp"
#include "units/units.hpp"
#include "units/units_avx.hpp"

#include <complex>

namespace fractal {
class DisplayToComplexCoordinates {
    const DisplayDomain DISPLAY_DOMAIN;
    complex_underlying real_scaling_factor_;
    complex_underlying imaginary_scaling_factor_;
    complex_coordinate complex_domain_start_;

    static complex_underlying real_scaling_factor(
        const display_coordinate& display_top_right,
        const complex_domain& complex_domain
    )
    {
        complex_underlying real_d_size = real_domain_size(complex_domain);
        return real_d_size / static_cast<complex_underlying>(display_top_right.x);
    }

    static complex_underlying imaginary_scaling_factor(
        const display_coordinate& display_top_right,
        const complex_domain& complex_domain
    )
    {
        complex_underlying imaginary_d_size = imaginary_domain_size(complex_domain);
        return imaginary_d_size / static_cast<complex_underlying>(display_top_right.y);
    }

    static complex_coordinate to_complex(display_coordinate coordinate)
    {
        return {
            static_cast<complex_underlying>(coordinate.x),
            static_cast<complex_underlying>(coordinate.y)
        };
    }

public:
    DisplayToComplexCoordinates(
        const DisplayDomain& display_domain, const complex_domain& complex_domain
    ) :
        DISPLAY_DOMAIN{display_domain},
        real_scaling_factor_(
            real_scaling_factor(display_domain.get_end_coordinate(), complex_domain)
        ),
        imaginary_scaling_factor_(imaginary_scaling_factor(
            display_domain.get_end_coordinate(), complex_domain
        )),
        complex_domain_start_(complex_domain.start_coordinate)
    {}

    void update_display_domain(const DisplayDomain& new_domain)
    {
        complex_coordinate new_start =
            to_complex_projection(new_domain.get_start_coordinate());
        complex_coordinate new_end =
            to_complex_projection(new_domain.get_end_coordinate());
        complex_domain new_complex_domain = {new_start, new_end};

        real_scaling_factor_ = real_scaling_factor(
            DISPLAY_DOMAIN.get_end_coordinate(), new_complex_domain
        );
        imaginary_scaling_factor_ = imaginary_scaling_factor(
            DISPLAY_DOMAIN.get_end_coordinate(), new_complex_domain
        );
        complex_domain_start_ = new_complex_domain.start_coordinate;
    }

    complex_coordinate to_complex_projection(display_coordinate display_coord)
    {
        std::complex<complex_underlying> raw_complex_coord = to_complex(display_coord);
        std::complex offset = {
            real_scaling_factor_ * raw_complex_coord.real(),
            imaginary_scaling_factor_ * raw_complex_coord.imag()
        };
        return complex_domain_start_ + offset;
    }

    avx512_complex to_complex_projections(display_coordinate display_coord)
    {
        std::complex<complex_underlying> raw_complex_coord = to_complex(display_coord);
        avx512_complex complex{};
        for (uint8_t i = 0; i < 8; i++) {
            complex.real[i] = complex_domain_start_.real()
                              + (raw_complex_coord.real() + i) * real_scaling_factor_;
            complex.imaginary[i] =
                complex_domain_start_.imag()
                + (raw_complex_coord.imag()) * imaginary_scaling_factor_;
        }
        return complex;
    }
};
} // namespace fractal
