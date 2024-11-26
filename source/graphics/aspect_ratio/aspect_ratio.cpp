#include "aspect_ratio.hpp"

namespace fractal {

display_coordinate calculate_rectangle_end_point(
    display_coordinate start, display_coordinate current, float target_aspect_ratio
)
{
    auto width = static_cast<float>(std::abs(current.x - start.x));
    auto height = static_cast<float>(std::abs(current.y - start.y));

    // Adjust the dimensions to maintain the target aspect ratio
    if (width / height > target_aspect_ratio) {
        // Too wide, adjust width
        width = height * target_aspect_ratio;
    }
    else {
        // Too tall, adjust height
        height = width / target_aspect_ratio;
    }

    auto new_x = static_cast<float>(std::min(current.x, start.x));
    auto new_y = static_cast<float>(std::min(current.y, start.y));

    // Adjust the top-left corner based on new dimensions
    if (current.x < start.x) {
        new_x = static_cast<float>(start.x) - width;
    }
    if (current.y < start.y) {
        new_y = static_cast<float>(start.y) - height;
    }

    // Return the top-left and bottom-right corners as a pair of sf::Vector2f
    return {new_x + width, new_y + height};
}
} // namespace fractal
