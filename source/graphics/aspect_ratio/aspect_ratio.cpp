#include "aspect_ratio.hpp"

#include "units/display_domain.hpp"

namespace fractal {

DisplayDomain calculate_rectangle_end_points(
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

    auto x_pos1 = static_cast<float>(start.x);
    auto y_pos1 = static_cast<float>(start.y);
    auto x_pos2 = x_pos1 + width;
    auto y_pos2 = y_pos1 + height;

    bool flipped_horizontal = start.x > current.x;
    bool flipped_vertical = start.y > current.y;

    if (flipped_vertical) {
        y_pos1 -= height;
        y_pos2 -= height;
    }
    if (flipped_horizontal) {
        x_pos1 -= width;
        x_pos2 -= width;
    }

    display_coordinate top_left{
        static_cast<uint16_t>(x_pos1), static_cast<uint16_t>(y_pos1)
    };
    display_coordinate bottom_right{
        static_cast<uint16_t>(x_pos2), static_cast<uint16_t>(y_pos2)
    };

    return {top_left, bottom_right};
}
} // namespace fractal
