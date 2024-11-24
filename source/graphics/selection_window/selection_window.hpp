#pragma once

#include "graphics/display_event_observer.hpp"

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

    void on_mouse_moved(const sf::Event::MouseMoveEvent& event) override;

    void on_mouse_button_pressed(const sf::Event::MouseButtonEvent& event) override;

    void on_mouse_button_released(const sf::Event::MouseButtonEvent& event) override;

    std::optional<std::unique_ptr<sf::Drawable>> get_drawable() override;
};
} // namespace fractal
