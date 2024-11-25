#pragma once

#include "config.hpp"
#include "coordinates.hpp"
#include "graphics/display_to_complex.hpp"
#include "units.hpp"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace fractal {
class MandelbrotWindow {
    using arr = std::array<std::array<float, WINDOW_HEIGHT>, WINDOW_WIDTH + 8>;
    DisplayToComplexCoordinates to_complex_;

    void draw_coordinate_(
        display_coordinate display_coord, const avx512_complex& complex_coords, arr& ref
    );

    void set_pixel_color(display_coordinate coordinate, float iteration_ratio);

public:
    MandelbrotWindow(display_domain display_domain, complex_domain complex_domain);

    arr
    calculate_(display_domain full_display_domain, display_domain new_domain_selection);
};
} // namespace fractal
