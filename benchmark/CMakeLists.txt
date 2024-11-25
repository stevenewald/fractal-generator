project(FractalBenchmarks LANGUAGES CXX)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()
# ---- Dependencies ----

find_package(benchmark REQUIRED)

# ---- Benchmarks ----
add_executable(fractal_benchmarks
  source/bench.cpp
  source/main.cpp
)

target_include_directories(
  fractal_benchmarks ${warning_guard}
  PUBLIC
  "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>"
)

target_link_libraries(
  fractal_benchmarks PRIVATE
  fractal-generator_lib
  sfml-graphics
  benchmark::benchmark
)

# Enable lto for release builds
include(CheckIPOSupported)
check_ipo_supported(RESULT lto_supported OUTPUT error)
if(lto_supported)
  set_target_properties(fractal_benchmarks PROPERTIES INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
else()
    message(WARNING "LTO is not supported: ${error}")
endif()

target_compile_features(fractal_benchmarks PRIVATE cxx_std_20)
