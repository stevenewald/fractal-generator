#include "basic_display.hpp"

#include "coordinates.hpp"
#include "graphics/color_conversions.hpp"

#include <fmt/format.h>

#include <cmath>

namespace fractal {
void BasicDisplay::set_pixel(display_coordinate coordinate, uint16_t value)
{
    auto tuple = number_to_rgb(value);

    image_.setPixel(
        static_cast<unsigned int>(coordinate.first),
        static_cast<unsigned int>(coordinate.second),
        sf::Color(std::get<0>(tuple), std::get<1>(tuple), std::get<2>(tuple))
    );
}

display_coordinate calculate_end_points(
    display_coordinate start, display_coordinate current,
    float target_aspect_ratio = 800.0f / 600.0f
)
{
    auto width = static_cast<float>(std::abs(current.first - start.first));
    auto height = static_cast<float>(std::abs(current.second - start.second));

    // Adjust the dimensions to maintain the target aspect ratio
    if (width / height > target_aspect_ratio) {
        // Too wide, adjust width
        width = height * target_aspect_ratio;
    }
    else {
        // Too tall, adjust height
        height = width / target_aspect_ratio;
    }

    auto x = static_cast<float>(std::min(current.first, start.first));
    auto y = static_cast<float>(std::min(current.second, start.second));

    // Adjust the top-left corner based on new dimensions
    if (current.first < start.first) {
        x = static_cast<float>(start.first) - width;
    }
    if (current.second < start.second) {
        y = static_cast<float>(start.second) - height;
    }

    // Return the top-left and bottom-right corners as a pair of sf::Vector2f
    return {x + width, y + height};
}

void BasicDisplay::display_window()
{
    texture_.loadFromImage(image_);
    sf::Sprite sprite{texture_};
    bool left_mouse_down{};
    float selection_start_x{};
    float selection_start_y{};
    float mouse_x{};
    float mouse_y{};

    while (window_.isOpen()) {
        sf::Event event{};
        while (window_.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window_.close();
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
                default:
                    break;

                case sf::Event::MouseButtonReleased:
                    if (event.mouseButton.button != sf::Mouse::Left) {
                        break;
                    }
                    auto ends = calculate_end_points(
                        {selection_start_x, selection_start_y}, {mouse_x, mouse_y}
                    );
                    on_resize_(
                        sf::Vector2f(
                            std::min(mouse_x, selection_start_x),
                            std::min(mouse_y, selection_start_y)
                        ),
                        sf::Vector2f(ends.first, ends.second)
                    );
                    left_mouse_down = false;
                    return;
            }
        }

        window_.clear(sf::Color::Black);

        window_.draw(sprite);
        if (left_mouse_down) {
            display_coordinate end_point = calculate_end_points(
                {selection_start_x, selection_start_y}, {mouse_x, mouse_y}
            );

            sf::RectangleShape selection_rectangle(
                {end_point.first - selection_start_x,
                 end_point.second - selection_start_y}
            );
            selection_rectangle.setPosition(selection_start_x, selection_start_y);
            selection_rectangle.setFillColor(sf::Color(255, 0, 0, 127));
            window_.draw(selection_rectangle);
        }

        window_.display();
    }
}
} // namespace fractal
