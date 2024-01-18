#pragma once

#include "core/rendering/texture.h"
#include "core/rendering/window.h"
#include "core/rendering/shader.h"
#include "core/color.h"

namespace crystal {
    typedef struct element_drawing_data {
        unsigned int vbo;
        unsigned int vao;
        unsigned int ebo;
    } element_drawing_data;

    enum VSyncMode {
        OFF = 0,
        ON = 1,
        DRAW = 2
    };

    class RenderingServer {
        private:
            static Window *_primary_window;
            static Color _clear_color;
            static VSyncMode _vsync_mode;

            static void _window_resize_callback(GLFWwindow *window, int width, int height);

            // GL stuff

            static void _create_native_shader(unsigned int *id, int type, const char *source);
            static void _create_default_quad(void);

            static element_drawing_data _quad_data;
        public:
            static native_shader shader_in_use;
            static glm::mat4 default_projection;

            RenderingServer() = delete;

            static void init(void);
            static void dispose(void);

            static void clear(void);
            static void present(void);

            static void use_native_shader(const native_shader shader);
            static native_shader create_native_shader(const char *fragment_source, const char *vertex_source);
            static void dispose_native_shader(const native_shader shader);

            static void activate_texture(const int texture);

            static void bind_texture(Texture *texture);
            static void use_shader(Shader *shader);

            static void draw_quad(void);

            static double get_monitor_refresh_rate(void);

            // Internal variables you can only get

            static Window *get_primary_window(void);

            static void create_primary_window(const glm::uvec2 size, const char *title);

            // Internal variables you can set

            static VSyncMode get_vsync_mode(void);
            static void set_vsync_mode(const VSyncMode vsync_mode);

            static Color get_clear_color(void);
            static void set_clear_color(const Color clear_color);
    };
}