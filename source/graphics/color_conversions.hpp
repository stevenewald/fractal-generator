#pragma once

#include <cstdint>

#include <tuple>

namespace fractal {
std::tuple<uint16_t, uint16_t, uint16_t>
hsv_to_rgb(float hue, float saturation, float value);

std::tuple<uint16_t, uint16_t, uint16_t> number_to_rgb(uint16_t number);
} // namespace fractal
