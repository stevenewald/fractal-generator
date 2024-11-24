#include "graphics/display/display.hpp"
#include "graphics/selection_window/selection_window.hpp"
#include "mandelbrot/mandelbrot_window.hpp"

#include <argparse/argparse.hpp>

namespace fractal {

void display_mandelbrot()
{
    PixelDisplay display;

    display.add_observer(std::make_unique<MandelbrotWindow>());
    display.add_observer(std::make_unique<SelectionWindow>());

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
