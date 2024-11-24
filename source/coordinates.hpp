#pragma once

#include "units.hpp"

#include <complex>

namespace fractal {
using display_coordinate = std::pair<uint16_t, uint16_t>;
using complex_coordinate = std::complex<complex_underlying>;

struct display_domain {
    display_coordinate start_coordinate;
    display_coordinate end_coordinate;

    class DisplayCoordinateIterator {
        display_coordinate current_coordinate_;
        display_coordinate end_coordinate_;

    public:
        explicit DisplayCoordinateIterator(const display_domain& domain) :
            current_coordinate_{domain.start_coordinate},
            end_coordinate_{domain.end_coordinate}
        {}

        const display_coordinate& operator*() const { return current_coordinate_; }

        DisplayCoordinateIterator& operator++()
        {
            if (current_coordinate_.first == end_coordinate_.first) [[unlikely]] {
                current_coordinate_.first = 0;
                current_coordinate_.second++;
            }
            else {
                current_coordinate_.first++;
            }
            return *this;
        }

        bool operator==(const DisplayCoordinateIterator&) const = default;
    };

    DisplayCoordinateIterator begin() const { return DisplayCoordinateIterator{*this}; }

    DisplayCoordinateIterator end() const
    {
        return DisplayCoordinateIterator{
            display_domain{end_coordinate, end_coordinate}
        };
    }
};

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
