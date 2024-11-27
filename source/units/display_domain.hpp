#pragma once

#include "coordinates.hpp"
#include "util.hpp"

#include <iterator>

namespace fractal {

class DisplayDomain {
    const display_coordinate START_COORDINATE;
    const display_coordinate END_COORDINATE;

public:
    DisplayDomain(display_coordinate start, display_coordinate end) :
        START_COORDINATE{start}, END_COORDINATE{end}
    {
        assert_true(start.x <= end.x);
        assert_true(start.y <= end.y);
    }

    const display_coordinate& get_start_coordinate() const { return START_COORDINATE; }

    const display_coordinate& get_end_coordinate() const { return END_COORDINATE; }

    class DisplayCoordinateIterator {
        const uint32_t GRID_WIDTH;
        uint32_t current_coordinate_;

    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = display_coordinate;
        using difference_type = std::uint32_t;
        using pointer = display_coordinate*;
        using reference = display_coordinate&;

        explicit DisplayCoordinateIterator(const DisplayDomain& domain) :
            GRID_WIDTH{domain.get_end_coordinate().x + 1u},
            current_coordinate_{
                decay_2d_coordinate(domain.get_start_coordinate(), GRID_WIDTH)
            }
        {}

        value_type operator*() const
        {
            return generate_1d_coordinate(current_coordinate_, GRID_WIDTH);
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

    uint32_t width() const { return END_COORDINATE.x - START_COORDINATE.x; }

    uint32_t height() const { return END_COORDINATE.y - START_COORDINATE.y; }

    DisplayCoordinateIterator begin() const { return DisplayCoordinateIterator{*this}; }

    DisplayCoordinateIterator end() const
    {
        return DisplayCoordinateIterator{
            DisplayDomain{END_COORDINATE, END_COORDINATE}
        };
    }

    std::uint32_t size() const { return END_COORDINATE.x * END_COORDINATE.y; }
};

} // namespace fractal
