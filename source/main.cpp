#include "graphics/basic_display.hpp"

void display_line()
{
    fractal::BasicDisplay display;
    for (std::size_t i = 0; i < 100; i++) {
        display.set_pixel(100, 100 + i, 255);
    }
    display.display_window();
}

#include "lib.hpp"

#include <argparse/argparse.hpp>

#include <complex>

#include <exception>
#include <iostream>
#include <string>

constexpr double divergence_norm = 4;
constexpr double x_dim = 2;
constexpr double y_dim = 2;
constexpr int max_iterations = 50;

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

    while (iterations < max_iters && std::norm(z_n) < divergence_norm) {
        z_n = step(z_n, constant);
        ++iterations;
    }

    return iterations;
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

    auto width = static_cast<double>(program.get<int>("width"));
    auto height = static_cast<double>(program.get<int>("height"));

    auto x_step = x_dim * 2 / width;
    auto y_step = y_dim * 2 / height;

    std::complex<double> constant = {1.0 / 4, 0};

    for (auto y = -y_dim; y < y_dim; y += y_step) {
        for (auto x = -x_dim; x < x_dim; x += x_step) {
            auto iterations = compute_iterations({x, y}, constant, max_iterations);
            std::cout << iterations << ' ';
        }
        std::cout << '\n';
    }

    return 0;
}
