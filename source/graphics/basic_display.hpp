#pragma once

#include "config.hpp"
#include "graphics/display_event_observer.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>

#include <memory>
#include <utility>
#include <vector>

namespace fractal {
class BasicDisplay {
    sf::RenderWindow window_{sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML Window"};
    std::vector<std::shared_ptr<DisplayEventObserver>> observers_;

public:
    explicit BasicDisplay() { window_.setFramerateLimit(FRAME_RATE); }

    void add_observer(std::shared_ptr<DisplayEventObserver> observer)
    {
        observers_.push_back(std::move(observer));
    }

    void display_window();
};
} // namespace fractal
