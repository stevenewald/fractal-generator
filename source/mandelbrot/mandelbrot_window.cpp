#include "mandelbrot_window.hpp"

#include "config.hpp"
#include "equations.hpp"
#include "graphics/display_to_complex.hpp"
#include "units/coordinates.hpp"
#include "units/units.hpp"

#include <fmt/format.h>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <cassert>
#include <chrono>

#include <future>
#include <iostream>

namespace fractal {

std::unique_ptr<MandelbrotWindow::pixel_iteration_counts> MandelbrotWindow::calculate_(
    const DisplayDomain& full_display_domain, const DisplayDomain& new_domain_selection
)
{
    to_complex_.update_display_domain(new_domain_selection);

    auto process_coordinates = [&](display_coordinate coord) {
        return compute_iterations(
            {}, to_complex_.to_complex_projections(coord), MANDELBROT_MAX_ITERATIONS
        );
    };

    auto ret = std::make_unique<pixel_iteration_counts>();
    auto process_chunk = [&](DisplayDomain::DisplayCoordinateIterator start,
                             DisplayDomain::DisplayCoordinateIterator end) {
        for (auto it = start; it != end; it += 8) {
            std::array<iteration_count, 8> t = process_coordinates(*it);
            std::copy(t.begin(), t.end(), ret->begin() + it.get_underlying());
        }
    };

    constexpr uint32_t total = WINDOW_WIDTH * WINDOW_HEIGHT;

    constexpr uint32_t chunks = 200;
    constexpr uint32_t step = total / chunks;

    static_assert(step % WINDOW_WIDTH == 0 && step % 8 == 0);

    std::vector<std::future<void>> futures;

    auto start = std::chrono::high_resolution_clock::now();

    for (uint32_t chunk = 0; chunk < chunks; chunk++) {
        DisplayDomain::DisplayCoordinateIterator it_start =
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
    const DisplayDomain& display_domain, const complex_domain& complex_domain
) : to_complex_{display_domain, complex_domain}
{}

} // namespace fractal
