#include "config.hpp"
#include "coordinates.hpp"
#include "graphics/basic_display.hpp"
#include "graphics/display_to_complex.hpp"

#include <argparse/argparse.hpp>

#include <complex>

#include <algorithm>
#include <limits>

namespace fractal {

constexpr double DIVERGENCE_NORM = 4;
constexpr display_domain DISPLAY_DOMAIN{
    {0,   0  },
    {799, 599}
};
constexpr complex_domain COMPLEX_DOMAIN{
    {-2, -1.5},
    {1,  1.5 }
};
constexpr std::size_t MAX_ITERATIONS = 50;

// https://en.wikipedia.org/wiki/Mandelbrot_set#Formal_definition
std::complex<double> step(std::complex<double> z_n, std::complex<double> constant)
{
    return z_n * z_n + constant;
}

std::size_t compute_iterations(
    std::complex<double> z_0, std::complex<double> constant, std::size_t max_iters
)
{
    std::size_t iterations = 0;
    std::complex<double> z_n = z_0;

    while (iterations < max_iters && std::norm(z_n) < DIVERGENCE_NORM) {
        z_n = step(z_n, constant);
        ++iterations;
    }

    return iterations;
}

void display_mandelbrot()
{
    complex_domain domain = COMPLEX_DOMAIN;
    DisplayToComplexCoordinates to_complex{DISPLAY_DOMAIN.end_coordinate, domain};

    auto on_resize = [&](sf::Vector2f first, sf::Vector2f second) {
        complex_coordinate top = to_complex.to_complex_projection({first.x, first.y});
        complex_coordinate bottom =
            to_complex.to_complex_projection({second.x, second.y});
        to_complex = {
            DISPLAY_DOMAIN.end_coordinate, {top, bottom}
        };
    };
    BasicDisplay display(on_resize);

    while (true) {
        auto process_coordinate = [&](const display_coordinate& coord) {
            auto complex_coord = to_complex.to_complex_projection(coord);

            // Compute the number of iterations
            auto iterations = compute_iterations({0, 0}, complex_coord, MAX_ITERATIONS);

            display.set_pixel(
                coord, static_cast<uint16_t>(
                           (static_cast<float>(iterations)
                            / static_cast<float>(MAX_ITERATIONS))
                           * std::numeric_limits<uint16_t>::max()
                       )
            );
        };

        std::for_each(DISPLAY_DOMAIN.begin(), DISPLAY_DOMAIN.end(), process_coordinate);

        display.display_window();
    }
}
} // namespace fractal

int main()
{
    // TODO: actually use this
    /*
    argparse::ArgumentParser program(lib.name);

    program.add_argument("width")
        .help("Horizontal resolution")
        .default_value(800)
        .scan<'i', int>();

    program.add_argument("height")
        .help("Vertical resolution")
        .default_value(600)
        .scan<'i', int>();

    try {
        program.parse_args(argc, argv);
    } catch (const std::exception& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        return 1;
    }

    auto width = program.get<int>("width");
    auto height = program.get<int>("height");
    */

    fractal::display_mandelbrot();

    return 0;
}
