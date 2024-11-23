#pragma once

#include "config.hpp"

#include <cstdint>

#include <array>

namespace fractal {
class BasicDisplay {
    std::array<std::array<uint8_t, WINDOW_HEIGHT>, WINDOW_WIDTH> pixels_{};

public:
    void set_pixel(std::size_t x_pos, std::size_t y_pos, std::uint8_t value);
    void display_window();
};
} // namespace fractal
