#pragma once

#include "config.hpp"
#include "units/coordinates.hpp"
#include "units/display_domain.hpp"

namespace fractal {
DisplayDomain calculate_rectangle_end_points(
    display_coordinate start, display_coordinate current,
    float target_aspect_ratio = static_cast<float>(WINDOW_WIDTH) / WINDOW_HEIGHT
);
} // namespace fractal
