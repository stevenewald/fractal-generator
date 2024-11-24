#include "aspect_ratio.hpp"

namespace fractal {

display_coordinate calculate_rectangle_end_point(
    display_coordinate start, display_coordinate current, float target_aspect_ratio
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
} // namespace fractal
