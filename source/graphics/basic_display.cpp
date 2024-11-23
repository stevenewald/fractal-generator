#include "basic_display.hpp"

#include "graphics/color_conversions.hpp"

#include <fmt/format.h>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <cmath>

namespace fractal {
void BasicDisplay::set_pixel(display_coordinate coordinate, uint16_t value)
{
    pixels_.at(coordinate.first).at(coordinate.second) = value;
}

std::tuple<int, int, int> interpolateColor(
    float t, std::tuple<int, int, int> color1, std::tuple<int, int, int> color2
)
{
    int r = std::get<0>(color1) + t * (std::get<0>(color2) - std::get<0>(color1));
    int g = std::get<1>(color1) + t * (std::get<1>(color2) - std::get<1>(color1));
    int b = std::get<2>(color1) + t * (std::get<2>(color2) - std::get<2>(color1));
    return {r, g, b};
}

void BasicDisplay::display_window()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML Window");

    window.setFramerateLimit(FRAME_RATE);

    sf::Image image;
    image.create(WINDOW_WIDTH, WINDOW_HEIGHT);
    for (std::size_t x_pos = 0; x_pos < WINDOW_WIDTH; ++x_pos) {
        for (std::size_t y_pos = 0; y_pos < WINDOW_HEIGHT; ++y_pos) {
            std::uint16_t pixel_value = pixels_.at(x_pos).at(y_pos);
            auto tuple = number_to_rgb(pixel_value);

            image.setPixel(
                static_cast<unsigned int>(x_pos), static_cast<unsigned int>(y_pos),
                sf::Color(std::get<0>(tuple), std::get<1>(tuple), std::get<2>(tuple))
            );
        }
    }

    sf::Texture texture;
    texture.loadFromImage(image);

    sf::Sprite sprite(texture);

    bool left_mouse_down{};
    float selection_start_x{};
    float selection_start_y{};
    float mouse_x{};
    float mouse_y{};

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::MouseMoved:
                    mouse_x = static_cast<float>(event.mouseMove.x);
                    mouse_y = static_cast<float>(event.mouseMove.y);
                    break;

                case sf::Event::MouseButtonPressed:
                    if (event.mouseButton.button != sf::Mouse::Left) {
                        break;
                    }
                    left_mouse_down = true;
                    selection_start_x = static_cast<float>(event.mouseButton.x);
                    selection_start_y = static_cast<float>(event.mouseButton.y);
                    break;

                case sf::Event::MouseButtonReleased:
                    if (event.mouseButton.button != sf::Mouse::Left) {
                        break;
                    }
                    on_resize_(
                        sf::Vector2f(
                            std::min(mouse_x, selection_start_x),
                            std::min(mouse_y, selection_start_y)
                        ),
                        sf::Vector2f(
                            std::max(mouse_x, selection_start_x),
                            std::max(mouse_y, selection_start_y)
                        )
                    );
                    left_mouse_down = false;
                    break;

                default:
                    break;
            }
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear(sf::Color::Black);

        window.draw(sprite);
        if (left_mouse_down) {
            sf::RectangleShape selection_rectangle(sf::Vector2f(
                std::abs(mouse_x - selection_start_x),
                std::abs(mouse_y - selection_start_y)
            ));
            selection_rectangle.setPosition(
                std::min(mouse_x, selection_start_x),
                std::min(mouse_y, selection_start_y)
            );

            selection_rectangle.setFillColor(sf::Color(255, 0, 0, 127));
            window.draw(selection_rectangle);
        }

        window.display();
    }
}
} // namespace fractal
