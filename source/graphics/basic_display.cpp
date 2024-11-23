#include "basic_display.hpp"

#include <SFML/Graphics.hpp>

namespace fractal {
void BasicDisplay::set_pixel(std::size_t x_pos, std::size_t y_pos, uint8_t value)
{
    pixels_.at(x_pos).at(y_pos) = value;
}

void BasicDisplay::display_window()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML Window");

    window.setFramerateLimit(FRAME_RATE);

    sf::Image image;
    image.create(WINDOW_WIDTH, WINDOW_HEIGHT);
    for (std::size_t x_pos = 0; x_pos < WINDOW_WIDTH; ++x_pos) {
        for (std::size_t y_pos = 0; y_pos < WINDOW_HEIGHT; ++y_pos) {
            std::uint8_t pixel_value = pixels_.at(x_pos).at(y_pos);
            image.setPixel(
                static_cast<unsigned int>(x_pos), static_cast<unsigned int>(y_pos),
                sf::Color(pixel_value, pixel_value, pixel_value)
            );
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
} // namespace fractal
