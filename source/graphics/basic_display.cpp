#include "basic_display.hpp"

#include <fmt/format.h>

#include <cmath>

namespace fractal {

void BasicDisplay::handle_event_(const sf::Event& event)
{
    switch (event.type) {
        case sf::Event::MouseMoved:
            for (const auto& observer : observers_) {
                observer->on_mouse_moved(event.mouseMove);
            }
            return;
        case sf::Event::MouseButtonPressed:
            std::for_each(
                observers_.begin(), observers_.end(),
                [&](const auto& observer) {
                    observer->on_mouse_button_pressed(event.mouseButton);
                }
            );
            return;
        case sf::Event::MouseButtonReleased:
            std::for_each(
                observers_.begin(), observers_.end(),
                [&](const auto& observer) {
                    observer->on_mouse_button_released(event.mouseButton);
                }
            );
            return;
        default:
            break;
    }
}

void BasicDisplay::add_observer(std::unique_ptr<DisplayEventObserver> observer)
{
    observers_.push_back(std::move(observer));
}

void BasicDisplay::poll_window_events()
{
    sf::Event event{};
    while (window_.pollEvent(event)) {
        handle_event_(event);
    }
}

void BasicDisplay::display_window()
{
    window_.clear(sf::Color::Black);

    auto draw_from_observer = [this](const auto& observer) {
        if (auto opt = observer->get_drawable(); opt) {
            window_.draw(*(opt.value()));
        }
    };

    std::for_each(observers_.begin(), observers_.end(), draw_from_observer);

    window_.display();
}
} // namespace fractal
