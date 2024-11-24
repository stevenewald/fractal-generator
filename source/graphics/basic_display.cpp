#include "basic_display.hpp"

#include "coordinates.hpp"
#include "ends.hpp"

#include <fmt/format.h>

#include <cmath>

namespace fractal {

void BasicDisplay::display_window()
{
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
                    std::for_each(
                        observers_.begin(), observers_.end(),
                        [&](const auto& observer) {
                            observer->on_mouse_button_pressed(event.mouseButton);
                        }
                    );
                    break;
                default:
                    break;

                case sf::Event::MouseButtonReleased:
                    if (event.mouseButton.button != sf::Mouse::Left) {
                        break;
                    }
                    std::for_each(
                        observers_.begin(), observers_.end(),
                        [&](const auto& observer) {
                            observer->on_mouse_button_released(event.mouseButton);
                        }
                    );
                    left_mouse_down = false;
                    break;
            }
        }

        window_.clear(sf::Color::Black);

        for (const auto& observer : observers_) {
            auto opt = observer->get_drawable();
            if (opt) {
                window_.draw(opt.value());
            }
        }

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
