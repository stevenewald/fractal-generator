#pragma once

#include "config.hpp"
#include "coordinates.hpp"
#include "graphics/aspect_ratio/aspect_ratio.hpp"
#include "graphics/color_conversions/color_conversions.hpp"
#include "graphics/display_event_observer.hpp"
#include "mandelbrot/mandelbrot_window.hpp"

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace fractal {
class Window : public DisplayEventObserver {
    const display_domain DISPLAY_DOMAIN;
    complex_domain complex_domain_;
    MandelbrotWindow mandelbrot_;

    sf::Image image_;
    sf::Texture texture_;
    int selection_start_x_{};
    int selection_start_y_{};

public:
    void set_pixel_color(display_coordinate coordinate, float iteration_ratio)
    {
        color output_color = ratio_to_rgb(iteration_ratio);

        image_.setPixel(
            coordinate.first, coordinate.second,
            sf::Color(output_color.red, output_color.green, output_color.blue)
        );
    }

    Window(display_domain display_domain, complex_domain complex_domain) :
        DISPLAY_DOMAIN{display_domain}, complex_domain_{complex_domain},
        mandelbrot_{display_domain, complex_domain}
    {
        image_.create(
            display_domain.end_coordinate.first + 1u,
            display_domain.end_coordinate.second + 1u
        );
        auto res = mandelbrot_.calculate_(display_domain, display_domain);
        for (size_t x = 0; x < WINDOW_WIDTH; x++) {
            for (size_t y = 0; y < WINDOW_HEIGHT; y++) {
                set_pixel_color({x, y}, res[x][y]);
            }
        }
    }

    void on_mouse_button_pressed(const sf::Event::MouseButtonEvent& event) override

    {
        selection_start_x_ = event.x;
        selection_start_y_ = event.y;
    }

    void on_mouse_button_released(const sf::Event::MouseButtonEvent& event) override
    {
        auto ends = calculate_rectangle_end_point(
            {selection_start_x_, selection_start_y_}, {event.x, event.y}
        );
        mandelbrot_.calculate_(
            DISPLAY_DOMAIN,
            {
                {selection_start_x_, selection_start_y_},
                ends
        }
        );
    }

    std::optional<std::unique_ptr<sf::Drawable>> get_drawable() override
    {
        texture_.loadFromImage(image_);
        return std::make_unique<sf::Sprite>(texture_);
    }
};
} // namespace fractal
