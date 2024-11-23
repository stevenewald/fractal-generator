#include "graphics/basic_display.hpp"
#include "lib.hpp"

#include <argparse/argparse.hpp>

#include <complex>

#include <exception>
#include <iostream>
#include <string>

constexpr double DIVERGENCE_NORM = 4;
constexpr double X_DIM = 2;
constexpr double Y_DIM = 2;
constexpr int MAX_ITERATIONS = 50;

// https://en.wikipedia.org/wiki/Mandelbrot_set#Formal_definition
std::complex<double> step(std::complex<double> z_n, std::complex<double> constant)
{
    return z_n * z_n + constant;
}

int compute_iterations(
    std::complex<double> z_0, std::complex<double> constant, int max_iters
)
{
    int iterations = 0;
    std::complex<double> z_n = z_0;

    while (iterations < max_iters && std::norm(z_n) < DIVERGENCE_NORM) {
        z_n = step(z_n, constant);
        ++iterations;
    }

    return iterations;
}

void display_line()
{
    fractal::BasicDisplay display;
    for (std::size_t i = 0; i < 100; i++) {
        display.set_pixel(100, 100 + i, 255);
    }
    display.display_window();
}

void display_julia(std::size_t width, std::size_t height, std::complex<double> constant)
{
    fractal::BasicDisplay display;

    auto x_step = X_DIM * 2 / static_cast<double>(width);
    auto y_step = Y_DIM * 2 / static_cast<double>(height);

    for (std::size_t j = 0; j < height; ++j) {
        for (std::size_t i = 0; i < width; ++i) {
            // Compute complex coordinates from pixel index
            double x = -X_DIM + i * x_step;
            double y = -Y_DIM + j * y_step;

            // Compute the number of iterations
            auto iterations = compute_iterations({x, y}, constant, MAX_ITERATIONS);

            // Draw the pixel
            display.set_pixel(
                i, j,
                static_cast<int>(iterations / static_cast<double>(MAX_ITERATIONS) * 255)
            );
        }
    }

    display.display_window();
}

int main(int argc, char** argv)
{
    auto const lib = library{};
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

    display_julia(width, height, {1.0 / 4, 0});

    return 0;
}
