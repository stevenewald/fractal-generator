#pragma once

#include "units/units.hpp"
#include "util.hpp"

#include <cassert>

namespace fractal {

color hsv_to_rgb(Hue hue, Percentage saturation, Percentage value);

color ratio_to_rgb(Percentage ratio);
} // namespace fractal
