#pragma once

#include "units.hpp"

namespace fractal {
color hsv_to_rgb(float hue, float saturation, float value);

color ratio_to_rgb(float ratio);
} // namespace fractal
