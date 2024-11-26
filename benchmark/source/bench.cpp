#include "config.hpp"
#include "units/coordinates.hpp"
#include "graphics/display_to_complex.hpp"
#include "mandelbrot/equations.hpp"
#include "mandelbrot/equations_simd.hpp"
#include "units/units.hpp"

#include <benchmark/benchmark.h>

using namespace fractal;

static void BM_GenerateMandelbrot(benchmark::State& state)
{
    DisplayDomain display = {
        {0,   0  },
        {800, 800}
    };
    complex_domain complex = START_COMPLEX_DOMAIN;
    DisplayToComplexCoordinates t{display, complex};

    std::int64_t prox = 0;
    for (auto _ : state) {
        for (auto it = display.begin(); it != display.end(); it += 1) {
            benchmark::DoNotOptimize(
                compute_iterations({}, t.to_complex_projection(*it), 100)
            );
        }
        prox += display.size();
    }
    state.SetItemsProcessed(prox);
}

static void BM_GenerateMandelbrotSimd(benchmark::State& state)
{
    DisplayDomain display = {
        {0,   0  },
        {800, 800}
    };
    complex_domain complex = START_COMPLEX_DOMAIN;
    avx512_complex start{};
    DisplayToComplexCoordinates t{display, complex};

    std::int64_t prox = 0;
    for (auto _ : state) {
        for (auto it = display.begin(); it != display.end(); it += 8) {
            benchmark::DoNotOptimize(
                compute_iterations(start, t.to_complex_projections(*it), 100)
            );
        }
        prox += display.size();
    }
    state.SetItemsProcessed(prox);
}

BENCHMARK(BM_GenerateMandelbrot)->Arg(0);
BENCHMARK(BM_GenerateMandelbrotSimd)->Arg(0);
// BENCHMARK(BM_GenerateMandelbrotSimd)->Arg(100000);
