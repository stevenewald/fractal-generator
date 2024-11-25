#include "mandelbrot_window.hpp"

#include "config.hpp"
#include "coordinates.hpp"
#include "equations_simd.hpp"
#include "graphics/display_to_complex.hpp"
#include "units.hpp"

#include <fmt/base.h>
#include <fmt/format.h>
#include <immintrin.h>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <cassert>
#include <chrono>

#include <future>
#include <iostream>

namespace fractal {
std::array<float, 8> MandelbrotWindow::draw_coordinate_(
    display_coordinate display_coord, const avx512_complex& complex_coords
)
{
    static constexpr avx512_complex START{};
    std::array<iteration_count, 8> iterations =
        compute_iterations(START, complex_coords, MANDELBROT_MAX_ITERATIONS);

    std::array<float, 8> ret{};
    for (size_t i = 0; i < 8; i++) {
        ret[i] = static_cast<float>(iterations[i]) / MANDELBROT_MAX_ITERATIONS;
        display_coord.first++;
    }
    return ret;
}

MandelbrotWindow::arr MandelbrotWindow::calculate_(
    display_domain full_display_domain, display_domain new_domain_selection
)
{
    to_complex_.update_display_domain(new_domain_selection);

    auto process_coordinates = [&](display_coordinate start_display_coord) {
        std::array<std::complex<complex_underlying>, 8> coords{};
        auto t = start_display_coord;
        for (size_t i = 0; i < 8; i++) {
            coords[i] = to_complex_.to_complex_projection(t);
            t.first++;
        }
        avx512_complex coords2{};
        for (size_t i = 0; i < 8; i++) {
            coords2.real[i] = coords[i].real();
            coords2.imaginary[i] = coords[i].imag();
        }
        return draw_coordinate_(start_display_coord, coords2);
    };

    arr ret;
    auto process_chunk = [&](display_domain::DisplayCoordinateIterator start,
                             display_domain::DisplayCoordinateIterator end) {
        for (auto it = start; it != end; it += 8) {
            auto pos = *it;
            std::array<float, 8> t = process_coordinates(pos);
            for (size_t i = 0; i < 8; i++) {
                ret[pos.first++][pos.second] = Ratio{t[i]};
            }
        }
    };

    constexpr uint32_t total = WINDOW_WIDTH * WINDOW_HEIGHT;

    constexpr uint32_t chunks = 200;
    constexpr uint32_t step = total / chunks;

    static_assert(step % WINDOW_WIDTH == 0 && step % 8 == 0);

    std::vector<std::future<void>> futures;

    auto start = std::chrono::high_resolution_clock::now();

    for (uint32_t chunk = 0; chunk < chunks; chunk++) {
        display_domain::DisplayCoordinateIterator it_start =
            full_display_domain.begin() + chunk * step;
        auto end = (chunk + 1) * step <= full_display_domain.size()
                       ? it_start + step
                       : full_display_domain.end();

        futures.push_back(std::async(std::launch::async, process_chunk, it_start, end));
    }

    for (const auto& future : futures) {
        future.wait();
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << fmt::format("Time elapsed: {}", time.count()) << "\n";
    return ret;
}

MandelbrotWindow::MandelbrotWindow(
    display_domain display_domain, complex_domain complex_domain
) : to_complex_{display_domain, complex_domain}
{}

} // namespace fractal
