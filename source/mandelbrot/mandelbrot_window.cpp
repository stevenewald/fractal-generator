#include "mandelbrot_window.hpp"

#include "config.hpp"
#include "coordinates.hpp"
#include "equations.hpp"
#include "graphics/aspect_ratio/aspect_ratio.hpp"
#include "graphics/color_conversions/color_conversions.hpp"
#include "graphics/display_to_complex.hpp"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <future>
#include <memory>
#include <optional>

namespace fractal {
void MandelbrotWindow::draw_coordinate_(
    const display_coordinate& display_coord, const complex_coordinate& complex_coord
)
{
    iteration_count iterations =
        compute_iterations({0, 0}, complex_coord, MANDELBROT_MAX_ITERATIONS);

    float iteration_ratio = static_cast<float>(iterations) / MANDELBROT_MAX_ITERATIONS;
    set_pixel_color(display_coord, iteration_ratio);
}

void MandelbrotWindow::on_resize_(display_domain new_domain_selection)
{
    DisplayToComplexCoordinates to_complex{DISPLAY_DOMAIN.end_coordinate, domain_};
    complex_coordinate new_top =
        to_complex.to_complex_projection(new_domain_selection.start_coordinate);
    complex_coordinate new_bottom =
        to_complex.to_complex_projection(new_domain_selection.end_coordinate);
    domain_ = {new_top, new_bottom};
    to_complex = {DISPLAY_DOMAIN.end_coordinate, domain_};

    auto process_coordinate = [&](const display_coordinate& display_coord) {
        auto complex_coord = to_complex.to_complex_projection(display_coord);
        draw_coordinate_(display_coord, complex_coord);
    };

    auto process_chunk = [&](display_domain::DisplayCoordinateIterator start,
                             display_domain::DisplayCoordinateIterator end) {
        std::for_each(start, end, process_coordinate);
    };

    uint32_t total = WINDOW_WIDTH * WINDOW_HEIGHT;
    uint32_t chunks = 32;
    uint32_t step = total / chunks;

    std::vector<std::future<void>> futures;

    for (uint32_t chunk = 0; chunk < chunks; chunk++) {
        display_domain::DisplayCoordinateIterator start =
            DISPLAY_DOMAIN.begin() + chunk * step;
        auto end = (chunk + 1) * step <= DISPLAY_DOMAIN.size() ? start + step
                                                               : DISPLAY_DOMAIN.end();

        futures.push_back(std::async(std::launch::async, process_chunk, start, end));
    }

    for (const auto& future : futures) {
        future.wait();
    }
}

MandelbrotWindow::MandelbrotWindow()
{
    image_.create(WINDOW_WIDTH, WINDOW_HEIGHT);
    on_resize_(DISPLAY_DOMAIN);
}

void MandelbrotWindow::on_mouse_button_pressed(const sf::Event::MouseButtonEvent& event)
{
    selection_start_x_ = event.x;
    selection_start_y_ = event.y;
}

void MandelbrotWindow::on_mouse_button_released(const sf::Event::MouseButtonEvent& event
)
{
    auto ends = calculate_rectangle_end_point(
        {selection_start_x_, selection_start_y_}, {event.x, event.y}
    );
    on_resize_({
        {selection_start_x_, selection_start_y_},
        ends
    });
}

void MandelbrotWindow::set_pixel_color(
    display_coordinate coordinate, float iteration_ratio
)
{
    color output_color = ratio_to_rgb(iteration_ratio);

    image_.setPixel(
        coordinate.first, coordinate.second,
        sf::Color(output_color.red, output_color.green, output_color.blue)
    );
}

std::optional<std::unique_ptr<sf::Drawable>> MandelbrotWindow::get_drawable()
{
    texture_.loadFromImage(image_);
    return std::make_unique<sf::Sprite>(texture_);
}
} // namespace fractal
