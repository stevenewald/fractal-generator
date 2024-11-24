#pragma once

#include "config.hpp"
#include "coordinates.hpp"
#include "graphics/color_conversions.hpp"
#include "graphics/display_event_observer.hpp"
#include "graphics/display_to_complex.hpp"
#include "graphics/ends.hpp"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <memory>
#include <optional>

namespace fractal {
using iteration_count = std::uint16_t;
constexpr iteration_count MAX_ITERATIONS = 256;
const complex_underlying DIVERGENCE_NORM = 4;

// https://en.wikipedia.org/wiki/Mandelbrot_set#Formal_definition
inline std::complex<complex_underlying>
step(std::complex<complex_underlying> z_n, std::complex<complex_underlying> constant)
{
    return z_n * z_n + constant;
}

inline iteration_count compute_iterations(
    std::complex<complex_underlying> z_0, std::complex<complex_underlying> constant,
    iteration_count max_iters
)
{
    iteration_count iterations = 0;
    std::complex<complex_underlying> z_n = z_0;

    while (iterations < max_iters && std::norm(z_n) < DIVERGENCE_NORM) {
        z_n = step(z_n, constant);
        ++iterations;
    }

    return iterations;
}

class Mandelbrot : public DisplayEventObserver {
    complex_domain domain_ = START_COMPLEX_DOMAIN;
    sf::Image image_;
    sf::Texture texture_;
    sf::Sprite sprite_{texture_};
    int start_x{};
    int start_y{};

public:
    Mandelbrot()
    {
        image_.create(WINDOW_WIDTH, WINDOW_HEIGHT);
        on_resize(DISPLAY_DOMAIN);
    }

    void on_mouse_button_pressed(const sf::Event::MouseButtonEvent& event) override
    {
        start_x = event.x;
        start_y = event.y;
    }

    void on_mouse_button_released(const sf::Event::MouseButtonEvent& event) override
    {
        auto ends = calculate_end_points({start_x, start_y}, {event.x, event.y});
        on_resize({
            {start_x, start_y},
            ends
        });
    }

    void set_pixel(display_coordinate coordinate, float ratio)
    {
        color output_color = ratio_to_rgb(ratio);

        image_.setPixel(
            coordinate.first, coordinate.second,
            sf::Color(output_color.red, output_color.green, output_color.blue)
        );
    }

    void on_resize(display_domain new_domain)
    {
        DisplayToComplexCoordinates to_complex{DISPLAY_DOMAIN.end_coordinate, domain_};
        complex_coordinate top =
            to_complex.to_complex_projection(new_domain.start_coordinate);
        complex_coordinate bottom =
            to_complex.to_complex_projection(new_domain.end_coordinate);
        domain_ = {top, bottom};
        to_complex = {DISPLAY_DOMAIN.end_coordinate, domain_};

        auto process_coordinate = [&](const display_coordinate& coord) {
            auto complex_coord = to_complex.to_complex_projection(coord);
            iteration_count iterations =
                compute_iterations({0, 0}, complex_coord, MAX_ITERATIONS);

            set_pixel(coord, static_cast<float>(iterations) / MAX_ITERATIONS);
        };

        std::for_each(DISPLAY_DOMAIN.begin(), DISPLAY_DOMAIN.end(), process_coordinate);
        flush_buffer();
    }

    void flush_buffer()
    {
        texture_.loadFromImage(image_);
        sprite_ = sf::Sprite{texture_};
    }

    std::optional<std::reference_wrapper<sf::Drawable>> get_drawable() override
    {
        return sprite_;
    }
};
} // namespace fractal
