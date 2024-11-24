#pragma once

#include "coordinates.hpp"

namespace fractal {
display_coordinate calculate_rectangle_end_point(
    display_coordinate start, display_coordinate current,
    float target_aspect_ratio = 800.0f / 600.0f
);
} // namespace fractal
