#include <stdio.h>

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include "core/rendering/rendering_server.h"

#include "core/engine.h"

namespace crystal {
    Window *RenderingServer::_primary_window;
    Color RenderingServer::_clear_color;
    VSyncMode RenderingServer::_vsync_mode;

    native_shader RenderingServer::shader_in_use;

    glm::mat4 RenderingServer::default_projection;
    element_drawing_data RenderingServer::_quad_data;

    void RenderingServer::init(void) {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }

    void RenderingServer::dispose(void) {
        glfwTerminate();
    }

    void RenderingServer::clear(void) {
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void RenderingServer::present(void) {
        glfwSwapBuffers(_primary_window->get_native_window());
    }

    void RenderingServer::use_native_shader(const native_shader shader) {
        // Prevents unneccesary performance hits I suppose.
        if (shader != shader_in_use) {
            glUseProgram(shader);
        }

        shader_in_use = shader;
    }

    void RenderingServer::_create_native_shader(unsigned int *id, int type, const char *source) {
        // Temporary variables
        unsigned int _gl_id;
        int _success;
        char _info_log[1024];

        *id = glCreateShader(type);
        _gl_id = *id;

        glShaderSource(_gl_id, 1, &source, NULL);
        glCompileShader(_gl_id);

        glGetShaderiv(_gl_id, GL_COMPILE_STATUS, &_success);

        if (!_success) {
            glGetShaderInfoLog(_gl_id, 1024, NULL, _info_log);
            fprintf(stderr, "ERROR Compiling Shader:\n%s\n", _info_log);
        }
    }

    native_shader RenderingServer::create_native_shader(const char *fragment_source, const char *vertex_source) {
        native_shader _native_shader;

        // Temporary variables
        unsigned int _fragment, _vertex;
        int _success;
        char _info_log[1024];

        _create_native_shader(&_fragment, GL_FRAGMENT_SHADER, fragment_source);
        _create_native_shader(&_vertex, GL_VERTEX_SHADER, vertex_source);

        _native_shader = glCreateProgram();
        glAttachShader(_native_shader, _fragment);
        glAttachShader(_native_shader, _vertex);
        glLinkProgram(_native_shader);

        glGetProgramiv(_native_shader, GL_LINK_STATUS, &_success);
        
        if (!_success) {
            glGetProgramInfoLog(_native_shader, 1024, NULL, _info_log);
            fprintf(stderr, "ERROR Linking Shader Program:\n%s\n", _info_log);
        }

        glDeleteShader(_fragment);
        glDeleteShader(_vertex);

        return _native_shader;
    }

    void RenderingServer::dispose_native_shader(const native_shader shader) {
        glDeleteProgram(shader);
    }

    void RenderingServer::activate_texture(const int texture) {
        glActiveTexture(texture);
    }

    void RenderingServer::bind_texture(Texture *texture) {
        if (texture == nullptr) {
            return;
        }
        
        texture->bind();
    }

    void RenderingServer::use_shader(Shader *shader) {
        shader->use();
    }

    void RenderingServer::draw_quad(void) {
        glBindVertexArray(_quad_data.vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    double RenderingServer::get_monitor_refresh_rate(void) {
        GLFWmonitor *primary_monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode *video_mode = glfwGetVideoMode(primary_monitor);

        return video_mode->refreshRate;
    }

    // internal variables you can only get
    Window *RenderingServer::get_primary_window(void) {
        return _primary_window;
    }

    void RenderingServer::create_primary_window(const glm::uvec2 size, const char *title) {
        _primary_window = new Window(size, title);
        glfwMakeContextCurrent(_primary_window->get_native_window());
        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        glfwSetWindowSizeCallback(_primary_window->get_native_window(), _window_resize_callback);

        activate_texture(GL_TEXTURE0);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        _create_default_quad();
    }

    void RenderingServer::_create_default_quad(void) {
        // These currently have no reason not to be hardcoded, so whatever.
        const float _vertices[16] = {
             // positions   // texture coords
             0.0f,  1.0f,   0.0f, 0.0f,   // top left
             0.0f,  0.0f,   0.0f, 1.0f,   // bottom left
             1.0f,  0.0f,   1.0f, 1.0f,   // bottom right
             1.0f,  1.0f,   1.0f, 0.0f    // top right 
        };

        const unsigned int _indices[6] = {
            0, 1, 3, // triangle #1
            1, 2, 3  // triangle #2
        };

        // Create VAO
        glGenVertexArrays(1, &_quad_data.vao);
        glGenBuffers(1, &_quad_data.vbo);
        glGenBuffers(1, &_quad_data.ebo);

        // Bind VAO
        glBindVertexArray(_quad_data.vao);

        // Buffer our VBO
        glBindBuffer(GL_ARRAY_BUFFER, _quad_data.vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);

        // Buffer our EBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _quad_data.ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices), _indices, GL_STATIC_DRAW);

        // Set vertex shader attributes
        // Position & UVs
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        default_projection = glm::ortho(0.0, (double)GAME_SIZE.x, (double)GAME_SIZE.y, 0.0, -1.0, 1.0);
    }

    // internal variables you can set
    VSyncMode RenderingServer::get_vsync_mode(void) {
        return _vsync_mode;
    }

    void RenderingServer::set_vsync_mode(const VSyncMode vsync_mode) {
        _vsync_mode = vsync_mode;

        switch (_vsync_mode) {
            case OFF:
            case ON:
                // VSyncMode::OFF = 0, VSyncMode::ON = 1
                // \_(:3)_/
                glfwSwapInterval(_vsync_mode);
                break;
            default:
                break;
        }
    }

    Color RenderingServer::get_clear_color(void) {
        return _clear_color;
    }

    void RenderingServer::set_clear_color(const Color clear_color) {
        _clear_color = clear_color;
        glClearColor(_clear_color.r, _clear_color.g, _clear_color.b, _clear_color.a);
    }

    void RenderingServer::_window_resize_callback(GLFWwindow *window, int width, int height) {
        glm::vec2 output_size = glm::vec2(0.0);
        glm::vec2 output_offset = glm::vec2(0.0);

        double game_ratio = (double)GAME_SIZE.x / (double)GAME_SIZE.y;
        double window_ratio = (double)width / (double)height;

        output_size.x = window_ratio > game_ratio ? (double)height * game_ratio : width;
        output_size.y = window_ratio < game_ratio ? (double)width / game_ratio : height;

        output_offset.x = (width - output_size.x) / 2.0;
        output_offset.y = (height - output_size.y) / 2.0;

        glViewport(floor(output_offset.x), floor(output_offset.y), floor(output_size.x), floor(output_size.y));
    }
}