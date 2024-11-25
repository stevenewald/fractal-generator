#pragma once

#include "units.hpp"
#include "util.hpp"

#include <cassert>

namespace fractal {

color hsv_to_rgb(Hue hue, Ratio saturation, Ratio value);

color ratio_to_rgb(Ratio ratio);
} // namespace fractal
