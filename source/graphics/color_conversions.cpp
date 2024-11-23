#include "color_conversions.hpp"

#include <cmath>

namespace fractal {
std::tuple<uint16_t, uint16_t, uint16_t>
hsv_to_rgb(float hue, float saturation, float value)
{
    float chroma = value * saturation;
    float hue_prime = hue / 60.0f;
    float uint16_termediate =
        chroma * (1 - static_cast<float>(std::fabs(std::fmod(hue_prime, 2) - 1)));
    float red_temp = 0.0f;
    float green_temp = 0.0f;
    float blue_temp = 0.0f;

    if (0 <= hue_prime && hue_prime < 1) {
        red_temp = chroma;
        green_temp = uint16_termediate;
        blue_temp = 0;
    }
    else if (1 <= hue_prime && hue_prime < 2) {
        red_temp = uint16_termediate;
        green_temp = chroma;
        blue_temp = 0;
    }
    else if (2 <= hue_prime && hue_prime < 3) {
        red_temp = 0;
        green_temp = chroma;
        blue_temp = uint16_termediate;
    }
    else if (3 <= hue_prime && hue_prime < 4) {
        red_temp = 0;
        green_temp = uint16_termediate;
        blue_temp = chroma;
    }
    else if (4 <= hue_prime && hue_prime < 5) {
        red_temp = uint16_termediate;
        green_temp = 0;
        blue_temp = chroma;
    }
    else if (5 <= hue_prime && hue_prime < 6) {
        red_temp = chroma;
        green_temp = 0;
        blue_temp = uint16_termediate;
    }

    float match_value = value - chroma;
    float red = red_temp + match_value;
    float green = green_temp + match_value;
    float blue = blue_temp + match_value;

    auto red_int = static_cast<uint16_t>(red * 255);
    auto green_int = static_cast<uint16_t>(green * 255);
    auto blue_int = static_cast<uint16_t>(blue * 255);

    return {red_int, green_int, blue_int};
}

std::tuple<uint16_t, uint16_t, uint16_t> number_to_rgb(uint16_t number)
{
    float hue = (static_cast<float>(number) / 65535.0f) * 360.0f;
    float saturation = 1.0f;
    float value = 1.0f;

    return hsv_to_rgb(hue, saturation, value);
}
} // namespace fractal
