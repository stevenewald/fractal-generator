cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    fractal-generator
    VERSION 0.1.0
    DESCRIPTION "Short description"
    HOMEPAGE_URL "https://example.com/"
    LANGUAGES CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -mavx512f -mavx512dq -mavx512vl -mavx512bf16")
# add_compile_options(-fno-inline -fno-omit-frame-pointer)


# ---- Declare library ----

add_library(
    fractal-generator_lib OBJECT
    source/graphics/display/display.cpp
    source/graphics/selection_window/selection_window.cpp
    source/mandelbrot/mandelbrot_window.cpp
    source/graphics/color_conversions/color_conversions.cpp
    source/graphics/aspect_ratio/aspect_ratio.cpp
    source/mandelbrot/equations_simd.cpp
    source/mandelbrot/equations.cpp
    source/units/coordinates.cpp
)

target_include_directories(
    fractal-generator_lib ${warning_guard}
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>"
)

target_compile_features(fractal-generator_lib PUBLIC cxx_std_23)

find_package(fmt REQUIRED)
find_package(argparse REQUIRED)
find_package(Boost REQUIRED)
find_package(SFML REQUIRED graphics CONFIG)
target_link_libraries(fractal-generator_lib PRIVATE fmt::fmt)
target_link_libraries(fractal-generator_lib PRIVATE argparse::argparse)
target_link_libraries(fractal-generator_lib PRIVATE boost::boost)
target_link_libraries(fractal-generator_lib PRIVATE sfml-graphics)

# ---- Declare executable ----

add_executable(fractal-generator_exe source/main.cpp)
add_executable(fractal-generator::exe ALIAS fractal-generator_exe)

set_property(TARGET fractal-generator_exe PROPERTY OUTPUT_NAME fractal-generator)

target_compile_features(fractal-generator_exe PRIVATE cxx_std_23)

target_link_libraries(fractal-generator_exe PRIVATE fmt::fmt)
target_link_libraries(fractal-generator_exe PRIVATE fractal-generator_lib)
target_link_libraries(fractal-generator_exe PRIVATE argparse::argparse)
target_link_libraries(fractal-generator_exe PRIVATE sfml-graphics)
target_link_libraries(fractal-generator_exe PRIVATE boost::boost)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Developer mode ----

if(NOT fractal-generator_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of fractal-generator"
  )
endif()

include(cmake/dev-mode.cmake)
