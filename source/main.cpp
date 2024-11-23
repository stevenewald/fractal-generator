#include <SFML/Graphics.hpp>

static constexpr auto x_res {800UZ};
static constexpr auto y_res {600UZ};
static constexpr auto frame_limit {60UZ};

int main()
{
  sf::RenderWindow window(sf::VideoMode(x_res, y_res), "SFML Window");

  window.setFramerateLimit(frame_limit);

  constexpr auto circle_size {50UZ};
  sf::CircleShape circle(circle_size);
  circle.setFillColor(sf::Color::Green);

  float window_width = static_cast<float>(window.getSize().x);
  float window_height = static_cast<float>(window.getSize().y);
  circle.setPosition((window_width - circle.getRadius() * 2) / 2,
                     (window_height - circle.getRadius() * 2) / 2);

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    window.clear(sf::Color::Black);

    window.draw(circle);

    window.display();
  }

  return 0;
}
