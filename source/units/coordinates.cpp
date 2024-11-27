#include "coordinates.hpp"

namespace fractal {
uint32_t
decay_2d_coordinate(const display_coordinate& coordinate, uint32_t display_width)
{
    return static_cast<uint32_t>(coordinate.x)
           + static_cast<uint32_t>(coordinate.y) * (display_width - 1u);
}

display_coordinate generate_1d_coordinate(uint32_t coordinate, uint32_t display_width)
{
    return {
        coordinate % display_width,
        (coordinate - (coordinate % display_width)) / display_width
    };
}
} // namespace fractal
