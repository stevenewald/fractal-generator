#pragma once

#include "units.hpp"

#include <complex>

#include <iterator>

namespace fractal {
using display_coordinate = std::pair<uint16_t, uint16_t>;
using complex_coordinate = std::complex<complex_underlying>;

struct display_domain {
    display_coordinate start_coordinate;
    display_coordinate end_coordinate;

    class DisplayCoordinateIterator {
        uint32_t grid_width_;
        uint32_t current_coordinate_;

        uint32_t decay_coordinate_(const display_coordinate& coordinate) const
        {
            return static_cast<uint32_t>(coordinate.first)
                   + static_cast<uint32_t>(coordinate.second) * grid_width_;
        }

        display_coordinate generate_coordinate_(uint32_t coordinate) const
        {
            return {
                coordinate % grid_width_,
                (coordinate - coordinate % grid_width_) / grid_width_
            };
        }

    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = display_coordinate;
        using difference_type = std::uint32_t;
        using pointer = display_coordinate*;
        using reference = display_coordinate&;

        explicit DisplayCoordinateIterator(const display_domain& domain) :
            grid_width_{domain.end_coordinate.first},
            current_coordinate_{decay_coordinate_(domain.start_coordinate)}
        {}

        value_type operator*() const
        {
            return generate_coordinate_(current_coordinate_);
        }

        DisplayCoordinateIterator& operator++()
        {
            ++current_coordinate_;
            return *this;
        }

        DisplayCoordinateIterator& operator--()
        {
            --current_coordinate_;
            return *this;
        }

        DisplayCoordinateIterator& operator+=(difference_type other)
        {
            current_coordinate_ += other;
            return *this;
        }

        DisplayCoordinateIterator operator-(difference_type other)
        {
            DisplayCoordinateIterator tmp = *this;
            tmp.current_coordinate_ -= other;
            return tmp;
        }

        DisplayCoordinateIterator operator+(difference_type other)
        {
            DisplayCoordinateIterator tmp = *this;
            tmp.current_coordinate_ += other;
            return tmp;
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

    std::uint32_t size() const { return end_coordinate.first * end_coordinate.second; }
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
