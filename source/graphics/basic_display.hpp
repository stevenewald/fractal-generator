#pragma once

#include "config.hpp"
#include "coordinates.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>

#include <cstdint>

#include <functional>
#include <utility>

namespace fractal {
class BasicDisplay {
    sf::RenderWindow window{sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML Window"};
    sf::Image image;
    sf::Texture texture;
    std::function<void(sf::Vector2f, sf::Vector2f)> on_resize_;

public:
    explicit BasicDisplay(std::function<void(sf::Vector2f, sf::Vector2f)> on_resize

    ) : on_resize_(std::move(on_resize))
    {
        window.setFramerateLimit(FRAME_RATE);
        image.create(WINDOW_WIDTH, WINDOW_HEIGHT);
    }

    void set_pixel(display_coordinate coordinate, uint16_t value);
    void display_window();
};
} // namespace fractal
