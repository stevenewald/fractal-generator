#pragma once

#include "config.hpp"
#include "graphics/display_to_complex.hpp"
#include "units/coordinates.hpp"
#include "units/units_avx.hpp"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace fractal {
class MandelbrotWindow {
    using arr = std::array<std::array<Percentage, WINDOW_HEIGHT + 8>, WINDOW_WIDTH + 8>;
    DisplayToComplexCoordinates to_complex_;

    static std::array<float, 8> draw_coordinate_(
        display_coordinate display_coord, const avx512_complex& complex_coords
    );

public:
    MandelbrotWindow(
        const DisplayDomain& display_domain, const complex_domain& complex_domain
    );

    arr calculate_(
        const DisplayDomain& full_display_domain,
        const DisplayDomain& new_domain_selection
    );
};
} // namespace fractal
