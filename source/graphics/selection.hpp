#pragma once

#include "coordinates.hpp"
#include "graphics/display_event_observer.hpp"
#include "graphics/ends.hpp"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>

namespace fractal {
class SelectionWindow : public DisplayEventObserver {
    float selection_start_x_{};
    float selection_start_y_{};
    float current_mouse_x_{};
    float current_mouse_y_{};
    bool left_mouse_down_ = false;

    void on_mouse_moved(const sf::Event::MouseMoveEvent& event) override
    {
        current_mouse_x_ = static_cast<float>(event.x);
        current_mouse_y_ = static_cast<float>(event.y);
    }

    void on_mouse_button_pressed(const sf::Event::MouseButtonEvent& event) override
    {
        if (event.button != sf::Mouse::Left) {
            return;
        }
        selection_start_x_ = static_cast<float>(event.x);
        selection_start_y_ = static_cast<float>(event.y);
        left_mouse_down_ = true;
    }

    void on_mouse_button_released(const sf::Event::MouseButtonEvent& event) override
    {
        if (event.button != sf::Mouse::Left) {
            return;
        }
        left_mouse_down_ = false;
    }

    std::optional<std::unique_ptr<sf::Drawable>> get_drawable() override
    {
        if (!left_mouse_down_)
            return std::nullopt;
        display_coordinate end_point = calculate_end_points(
            {selection_start_x_, selection_start_y_},
            {current_mouse_x_, current_mouse_y_}
        );

        auto rectangle = std::make_unique<sf::RectangleShape>(sf::Vector2f{
            static_cast<float>(end_point.first) - selection_start_x_,
            static_cast<float>(end_point.second) - selection_start_y_
        });
        rectangle->setPosition(selection_start_x_, selection_start_y_);
        rectangle->setFillColor(sf::Color(255, 0, 0, 127));
        return rectangle;
    }
};
} // namespace fractal
