#include "selection_window.hpp"

#include "graphics/aspect_ratio/aspect_ratio.hpp"
#include "units/coordinates.hpp"

namespace fractal {

void SelectionWindow::on_mouse_moved(const sf::Event::MouseMoveEvent& event)
{
    current_mouse_x_ = static_cast<float>(event.x);
    current_mouse_y_ = static_cast<float>(event.y);
}

void SelectionWindow::on_mouse_button_pressed(const sf::Event::MouseButtonEvent& event)
{
    if (event.button != sf::Mouse::Left) {
        return;
    }
    selection_start_x_ = static_cast<float>(event.x);
    selection_start_y_ = static_cast<float>(event.y);
    left_mouse_down_ = true;
}

void SelectionWindow::on_mouse_button_released(const sf::Event::MouseButtonEvent& event)
{
    if (event.button != sf::Mouse::Left) {
        return;
    }
    left_mouse_down_ = false;
}

std::optional<std::unique_ptr<sf::Drawable>> SelectionWindow::get_drawable()
{
    if (!left_mouse_down_)
        return std::nullopt;
    DisplayDomain new_selection = calculate_rectangle_end_points(
        {selection_start_x_, selection_start_y_}, {current_mouse_x_, current_mouse_y_}
    );

    auto rectangle = std::make_unique<sf::RectangleShape>(sf::Vector2f{
        static_cast<float>(new_selection.width()),
        static_cast<float>(new_selection.height())
    });
    auto [x, y] = new_selection.get_start_coordinate();
    rectangle->setPosition(x, y);
    rectangle->setFillColor(sf::Color(255, 0, 0, 127));
    return rectangle;
}
} // namespace fractal
