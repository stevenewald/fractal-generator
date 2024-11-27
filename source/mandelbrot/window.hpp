#pragma once

#include "graphics/aspect_ratio/aspect_ratio.hpp"
#include "graphics/color_conversions/color_conversions.hpp"
#include "graphics/display_event_observer.hpp"
#include "mandelbrot/mandelbrot_window.hpp"
#include "units/coordinates.hpp"
#include "units/display_domain.hpp"
#include "units/units.hpp"

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace fractal {
class Window : public DisplayEventObserver {
    const DisplayDomain DISPLAY_DOMAIN;
    complex_domain complex_domain_;
    MandelbrotWindow mandelbrot_;

    sf::Image image_;
    sf::Texture texture_;
    int selection_start_x_{};
    int selection_start_y_{};

public:
    void set_pixel_color(display_coordinate coordinate, Percentage iteration_ratio)
    {
        color output_color = ratio_to_rgb(iteration_ratio);

        image_.setPixel(
            coordinate.x, coordinate.y,
            sf::Color(output_color.red, output_color.green, output_color.blue)
        );
    }

    Window(DisplayDomain display_domain, complex_domain complex_domain) :
        DISPLAY_DOMAIN{display_domain}, complex_domain_{complex_domain},
        mandelbrot_{display_domain, complex_domain}
    {
        image_.create(
            display_domain.get_end_coordinate().x + 1u,
            display_domain.get_end_coordinate().y + 1u
        );

        auto res = mandelbrot_.calculate_(display_domain, display_domain);
        for (display_coordinate pos : DISPLAY_DOMAIN) {
            set_pixel_color(pos, res[pos.x][pos.y]);
        }
    }

    void on_mouse_button_pressed(const sf::Event::MouseButtonEvent& event) override

    {
        selection_start_x_ = event.x;
        selection_start_y_ = event.y;
    }

    void on_mouse_button_released(const sf::Event::MouseButtonEvent& event) override
    {
        DisplayDomain ends = calculate_rectangle_end_points(
            {selection_start_x_, selection_start_y_}, {event.x, event.y}
        );
        auto res = mandelbrot_.calculate_(DISPLAY_DOMAIN, ends);
        for (display_coordinate pos : DISPLAY_DOMAIN) {
            set_pixel_color(pos, res[pos.x][pos.y]);
        }
    }

    std::optional<std::unique_ptr<sf::Drawable>> get_drawable() override
    {
        texture_.loadFromImage(image_);
        return std::make_unique<sf::Sprite>(texture_);
    }
};
} // namespace fractal
