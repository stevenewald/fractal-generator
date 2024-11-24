#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Window/Event.hpp>

#include <memory>
#include <optional>

namespace fractal {
class DisplayEventObserver {
public:
    virtual void on_mouse_moved(const sf::Event::MouseMoveEvent&) {}

    virtual void on_mouse_button_pressed(const sf::Event::MouseButtonEvent&) {}

    virtual void on_mouse_button_released(const sf::Event::MouseButtonEvent&) {}

    virtual std::optional<std::reference_wrapper<sf::Drawable>> get_drawable() = 0;

    DisplayEventObserver() = default;
    DisplayEventObserver(DisplayEventObserver&&) = default;
    DisplayEventObserver(const DisplayEventObserver&) = default;
    DisplayEventObserver& operator=(const DisplayEventObserver&) = default;
    DisplayEventObserver& operator=(DisplayEventObserver&&) = default;
    virtual ~DisplayEventObserver() = default;
};
} // namespace fractal
