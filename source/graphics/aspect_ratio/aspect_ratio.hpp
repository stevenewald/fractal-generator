#pragma once

#include "config.hpp"
#include "units/coordinates.hpp"

namespace fractal {
display_coordinate calculate_rectangle_end_point(
    display_coordinate start, display_coordinate current,
    float target_aspect_ratio = static_cast<float>(WINDOW_WIDTH) / WINDOW_HEIGHT
);
} // namespace fractal
