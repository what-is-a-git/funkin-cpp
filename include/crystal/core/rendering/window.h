#pragma once

#define GLFW_INCLUDE_NONE
#include "glfw/glfw3.h"
#include "glm/vec2.hpp"

namespace crystal {
    class Window {
        private:
            GLFWwindow *_native_window;
            glm::uvec2 _position;
            glm::uvec2 _size;
            char *_title;
        public:
            Window(const glm::uvec2 size, const char *title);
            Window(const glm::uvec2 position, const glm::uvec2 size, const char *title);
            ~Window();

            // Position
            
            glm::uvec2 get_position(void);
            void set_position(const glm::uvec2 position);

            unsigned int get_x(void);
            unsigned int get_y(void);

            void set_x(const unsigned int x);
            void set_y(const unsigned int y);

            // Size

            glm::uvec2 get_size(void);
            void set_size(const glm::uvec2 size);

            unsigned int get_width(void);
            unsigned int get_height(void);

            void set_width(const unsigned int width);
            void set_height(const unsigned int height);

            // Title

            char *get_title(void);
            void set_title(char *title);

            // Icon

            void set_icon(const char *path);

            // Should Close

            bool get_should_close(void);
            void set_should_close(const bool should_close);

            GLFWwindow *get_native_window(void);
    };
}
