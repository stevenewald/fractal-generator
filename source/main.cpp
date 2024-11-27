#include "config.hpp"
#include "graphics/display/display.hpp"
#include "graphics/selection_window/selection_window.hpp"
#include "mandelbrot/mandelbrot_window.hpp"
#include "mandelbrot/window.hpp"
#include "units/coordinates.hpp"

#include <argparse/argparse.hpp>

namespace fractal {

void display_mandelbrot()
{
    DisplayDomain display_domain{
        {0,                0                },
        {WINDOW_WIDTH - 1, WINDOW_HEIGHT - 1}
    };
    complex_domain complex_domain = START_COMPLEX_DOMAIN;

    PixelDisplay display;
    display.add_drawable(std::make_unique<Window>(display_domain, complex_domain));
    display.add_drawable(std::make_unique<SelectionWindow>());

    while (true) {
        display.poll_window_events();
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
