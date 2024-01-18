#pragma once

#define COLOR_TRANSPARENT crystal::Color(0.0, 0.0, 0.0, 0.0)
#define COLOR_BLACK crystal::Color(0.0, 0.0, 0.0)
#define COLOR_WHITE crystal::Color(1.0, 1.0, 1.0)

// Reference to learnopengl.com
#define COLOR_LEARNOPENGL crystal::Color(0.2, 0.3, 0.3)

namespace crystal {
    class Color {
        public:
            double r;
            double g;
            double b;
            double a;

            Color();

            // 0 - 1 RGB & RGBA (float values like in native gl)

            Color(const double red, const double green, const double blue);
            Color(const double red, const double green, const double blue, const double alpha);

            // 0 - 255 RGB & RGBA (int values like in image editors)

            Color(const unsigned int red, const unsigned int green, const unsigned int blue);
            Color(const unsigned int red, const unsigned int green, const unsigned int blue, const unsigned int alpha);

            ~Color() = default;

            // 0 - 255 RGBA (int)

            unsigned int get_red_rgba(void);
            unsigned int get_green_rgba(void);
            unsigned int get_blue_rgba(void);
            unsigned int get_alpha_rgba(void);

            void set_red_rgba(const unsigned int red);
            void set_green_rgba(const unsigned int green);
            void set_blue_rgba(const unsigned int blue);
            void set_alpha_rgba(const unsigned int alpha);
    };
}