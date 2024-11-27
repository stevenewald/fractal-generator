# Parent project does not export its library target, so this CML implicitly
# depends on being added from it, i.e. the testing is done only from the build
# tree and is not feasible from an install location

project(fractal-generatorTests LANGUAGES CXX)

# ---- Dependencies ----

find_package(Catch2 REQUIRED)
include(Catch)

# ---- Tests ----

add_executable(fractal-generator_test source/fractal-generator_test.cpp)
target_link_libraries(
    fractal-generator_test PRIVATE
    fractal-generator_lib
    Catch2::Catch2WithMain
)
target_compile_features(fractal-generator_test PRIVATE cxx_std_23)

catch_discover_tests(fractal-generator_test)

# ---- End-of-file commands ----

add_folders(Test)
