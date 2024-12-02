#pragma once

#include "config.hpp"
#include "graphics/display_to_complex.hpp"
#include "units/coordinates.hpp"
#include "units/units_avx.hpp"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <memory>

namespace fractal {
class MandelbrotWindow {
    using arr = std::array<std::array<Percentage, WINDOW_HEIGHT>, WINDOW_WIDTH>;
    DisplayToComplexCoordinates to_complex_;

    static std::array<float, 8> draw_coordinate_(
        display_coordinate display_coord, const avx512_complex& complex_coords
    );

public:
    MandelbrotWindow(
        const DisplayDomain& display_domain, const complex_domain& complex_domain
    );

    std::unique_ptr<arr> calculate_(
        const DisplayDomain& full_display_domain,
        const DisplayDomain& new_domain_selection
    );
};
} // namespace fractal
