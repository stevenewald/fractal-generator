#pragma once

#include "coordinates.hpp"
#include "graphics/display_event_observer.hpp"
#include "graphics/display_to_complex.hpp"
#include "units.hpp"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <memory>
#include <optional>

namespace fractal {
class MandelbrotWindow : public DisplayEventObserver {
    const display_domain DISPLAY_DOMAIN;
    complex_domain complex_domain_;
    DisplayToComplexCoordinates to_complex_;
    sf::Image image_;
    sf::Texture texture_;
    int selection_start_x_{};
    int selection_start_y_{};

    void on_resize_(display_domain new_domain_selection);
    void draw_coordinate_(
        display_coordinate display_coord, const avx512_complex& complex_coords
    );

public:
    MandelbrotWindow(display_domain display_domain, complex_domain complex_domain);

    void on_mouse_button_pressed(const sf::Event::MouseButtonEvent& event) override;

    void on_mouse_button_released(const sf::Event::MouseButtonEvent& event) override;

    void set_pixel_color(display_coordinate coordinate, float iteration_ratio);

    std::optional<std::unique_ptr<sf::Drawable>> get_drawable() override;
};
} // namespace fractal
