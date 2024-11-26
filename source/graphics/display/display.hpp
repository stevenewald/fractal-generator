#pragma once

#include "config.hpp"
#include "graphics/display_event_observer.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>

#include <memory>
#include <vector>

namespace fractal {
class PixelDisplay {
    sf::RenderWindow window_{sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML Window"};
    std::vector<std::unique_ptr<DisplayEventObserver>> observers_;

    void handle_event_(const sf::Event& event);

public:
    explicit PixelDisplay();

    void add_drawable(std::unique_ptr<DisplayEventObserver> observer);

    void poll_window_events();
    void display_window();
};
} // namespace fractal
