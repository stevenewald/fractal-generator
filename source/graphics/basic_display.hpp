#pragma once

#include "config.hpp"

#include <SFML/System/Vector2.hpp>

#include <cstdint>

#include <array>
#include <functional>
#include <utility>

namespace fractal {
class BasicDisplay {
    std::array<std::array<uint8_t, WINDOW_HEIGHT>, WINDOW_WIDTH> pixels_{};
    std::function<void(sf::Vector2f, sf::Vector2f)> on_resize_;

public:
    explicit BasicDisplay(
        std::function<void(sf::Vector2f, sf::Vector2f)> on_resize = [](sf::Vector2f,
                                                                       sf::Vector2f) {}
    ) : on_resize_(std::move(on_resize))
    {}

    void set_pixel(std::size_t x_pos, std::size_t y_pos, std::uint8_t value);
    void display_window();
};
} // namespace fractal
