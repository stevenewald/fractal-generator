#include "color_conversions.hpp"

#include <fmt/format.h>

#include <cmath>

#include <stdexcept>

namespace fractal {
color hsv_to_rgb(Hue hue, Ratio saturation, Ratio value)
{
    float chroma = value.get_ratio() * saturation.get_ratio();
    float hue_prime = hue.get_hue() / 60.0f;
    float uint16_termediate =
        chroma * (1 - static_cast<float>(std::fabs(std::fmod(hue_prime, 2) - 1)));
    float red_temp = 0.0f;
    float green_temp = 0.0f;
    float blue_temp = 0.0f;

    if (hue_prime < 1) {
        red_temp = chroma;
        green_temp = uint16_termediate;
    }
    else if (hue_prime < 2) {
        red_temp = uint16_termediate;
        green_temp = chroma;
    }
    else if (hue_prime < 3) {
        green_temp = chroma;
        blue_temp = uint16_termediate;
    }
    else if (hue_prime < 4) {
        green_temp = uint16_termediate;
        blue_temp = chroma;
    }
    else if (hue_prime < 5) {
        red_temp = uint16_termediate;
        blue_temp = chroma;
    }
    else if (hue_prime < 6) {
        red_temp = chroma;
        blue_temp = uint16_termediate;
    }

    float match_value = value.get_ratio() - chroma;
    float red = red_temp + match_value;
    float green = green_temp + match_value;
    float blue = blue_temp + match_value;

    auto red_int = static_cast<uint8_t>(red * 255);
    auto green_int = static_cast<uint8_t>(green * 255);
    auto blue_int = static_cast<uint8_t>(blue * 255);

    return {red_int, green_int, blue_int};
}

color ratio_to_rgb(Ratio ratio)
{
    Hue hue{ratio.get_ratio() * 360.0f};
    Ratio saturation{1.0f};
    Ratio value{1.0f};

    return hsv_to_rgb(hue, saturation, value);
}
} // namespace fractal
