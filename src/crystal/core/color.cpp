#include <math.h>

#include "core/color.h"

namespace crystal {
    // pure white
    Color::Color() {
        r = 1.0;
        g = 1.0;
        b = 1.0;
        a = 1.0;
    }

    // fill in alpha automatically
    Color::Color(const double red, const double green, const double blue) {
        r = red;
        g = green;
        b = blue;
        a = 1.0;
    }

    Color::Color(const double red, const double green, const double blue, const double alpha) {
        r = red;
        g = green;
        b = blue;
        a = alpha;
    }

    // convert to 0 - 1
    Color::Color(const unsigned int red, const unsigned int green, const unsigned int blue) {
        r = red / 255.0;
        g = green / 255.0;
        b = blue / 255.0;
        a = 1.0;
    }

    // convert to 0 - 1 but with alpha
    Color::Color(const unsigned int red, const unsigned int green, const unsigned int blue, const unsigned int alpha) {
        r = red / 255.0;
        g = green / 255.0;
        b = blue / 255.0;
        a = alpha / 255.0;
    }

    // 0 - 255 rgba
    unsigned int Color::get_red_rgba(void) {
        return round(255.0 * r);
    }

    unsigned int Color::get_green_rgba(void) {
        return round(255.0 * g);
    }

    unsigned int Color::get_blue_rgba(void) {
        return round(255.0 * b);
    }

    unsigned int Color::get_alpha_rgba(void) {
        return round(255.0 * a);
    }

    void Color::set_red_rgba(const unsigned int red) {
        r = red / 255.0;
    }

    void Color::set_green_rgba(const unsigned int green) {
        g = green / 255.0;
    }

    void Color::set_blue_rgba(const unsigned int blue) {
        b = blue / 255.0;
    }

    void Color::set_alpha_rgba(const unsigned int alpha) {
        a = alpha / 255.0;
    }
}