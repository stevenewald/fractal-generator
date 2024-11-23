#include "lib.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Name is fractal-generator", "[library]")
{
    auto const lib = library{};
    REQUIRE(lib.name == "fractal-generator");
}
