#include "graphics/basic_display.hpp"

int main()
{
    fractal::BasicDisplay display;
    for (size_t i = 0; i < 100; i++) {
        display.set_pixel(100, 100 + i, 255);
    }
    display.display_window();
    return 0;
}
