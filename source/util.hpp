#pragma once

#include <cassert>

#include <stdexcept>
#include <utility>

namespace fractal {

constexpr void assert_true(bool expr)
{
#ifdef NDEBUG
    if (!expr) {
        std::unreachable();
    }
#else
    if (!expr) [[unlikely]] {
        throw std::runtime_error("Assertion error");
    }
#endif
}

} // namespace fractal
