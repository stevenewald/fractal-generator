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
    DisplayToComplexCoordinates to_complex_;

public:
    MandelbrotWindow(
        const DisplayDomain& display_domain, const complex_domain& complex_domain
    );

    using pixel_iteration_counts =
        std::array<iteration_count, WINDOW_HEIGHT * WINDOW_WIDTH>;
    std::unique_ptr<pixel_iteration_counts> calculate_(
        const DisplayDomain& full_display_domain,
        const DisplayDomain& new_domain_selection
    );
};
} // namespace fractal
