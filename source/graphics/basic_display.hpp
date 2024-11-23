#pragma once

#include "config.hpp"

#include <SFML/Graphics.hpp>

#include <cstddef>
#include <cstdint>

#include <array>

namespace fractal {
class BasicDisplay {
    std::array<std::array<uint8_t, WINDOW_HEIGHT>, WINDOW_WIDTH> pixels_{};

public:
    void set_pixel(std::size_t x_pos, std::size_t y_pos, uint8_t value)
    {
        pixels_.at(x_pos).at(y_pos) = value;
    }

    void display_window()
    {
        sf::RenderWindow window(
            sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML Window"
        );

        window.setFramerateLimit(FRAME_RATE);

        sf::Image image;
        image.create(WINDOW_WIDTH, WINDOW_HEIGHT);
        for (std::size_t x = 0; x < WINDOW_WIDTH; ++x) {
            for (std::size_t y = 0; y < WINDOW_HEIGHT; ++y) {
                uint8_t pixel_value = pixels_.at(x).at(y);
                image.setPixel(x, y, sf::Color(pixel_value, pixel_value, pixel_value));
            }
        }

        sf::Texture texture;
        texture.loadFromImage(image);

        sf::Sprite sprite(texture);

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
            }

            window.clear(sf::Color::Black);

            window.draw(sprite);

            window.display();
        }
    }
};
} // namespace fractal
