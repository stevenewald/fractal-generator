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

# ---- Declare library ----

add_library(
    fractal-generator_lib OBJECT
    source/lib.cpp
    source/graphics/basic_display.cpp
    source/graphics/color_conversions.cpp
)

target_include_directories(
    fractal-generator_lib ${warning_guard}
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/source>"
)

target_compile_features(fractal-generator_lib PUBLIC cxx_std_20)

find_package(fmt REQUIRED)
find_package(argparse REQUIRED)
target_link_libraries(fractal-generator_lib PUBLIC fmt::fmt)
target_link_libraries(fractal-generator_lib PUBLIC argparse::argparse)

find_package(SFML REQUIRED graphics CONFIG)
target_link_libraries(fractal-generator_lib PUBLIC sfml-graphics)

# ---- Declare executable ----

add_executable(fractal-generator_exe source/main.cpp)
add_executable(fractal-generator::exe ALIAS fractal-generator_exe)

set_property(TARGET fractal-generator_exe PROPERTY OUTPUT_NAME fractal-generator)

target_compile_features(fractal-generator_exe PRIVATE cxx_std_20)

target_link_libraries(fractal-generator_exe PRIVATE fractal-generator_lib)
target_link_libraries(fractal-generator_exe PUBLIC sfml-graphics)

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
