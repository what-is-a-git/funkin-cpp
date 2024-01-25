#include "core/rendering/window.h"

#include "stb/stb_image.h"

namespace crystal {
    Window::Window(const glm::uvec2 size, const char *title) {
        _native_window = glfwCreateWindow(size.x, size.y, title, NULL, NULL);
        _size = size;

        // we do this because we can't directly use _position's pointers ;-;
        int temp_x, temp_y;
        glfwGetWindowPos(_native_window, &temp_x, &temp_y);
        _position = glm::uvec2(temp_x, temp_y);
    }

    Window::Window(const glm::uvec2 position, const glm::uvec2 size, const char *title) {
        _native_window = glfwCreateWindow(size.x, size.y, title, NULL, NULL);
        glfwSetWindowPos(_native_window, position.x, position.y);
        _size = size;
        _position = position;
    }

    Window::~Window() {
        glfwDestroyWindow(_native_window);
    }

    // position
    glm::uvec2 Window::get_position(void) {
        return _position;
    }

    void Window::set_position(const glm::uvec2 position) {
        _position = position;
        glfwSetWindowPos(_native_window, _position.x, _position.y);
    }

    unsigned int Window::get_x(void) {
        return _position.x;
    }

    unsigned int Window::get_y(void) {
        return _position.y;
    }

    void Window::set_x(const unsigned int x) {
        glfwSetWindowPos(_native_window, x, _position.y);
        _position.x = x;
    }

    void Window::set_y(const unsigned int y) {
        glfwSetWindowPos(_native_window, _position.x, y);
        _position.y = y;
    }

    // size
    glm::uvec2 Window::get_size(void) {
        return _size;
    }

    void Window::set_size(const glm::uvec2 size) {
        _size = size;
        glfwSetWindowSize(_native_window, _size.x, _size.y);
    }

    unsigned int Window::get_width(void) {
        return _size.x;
    }

    unsigned int Window::get_height(void) {
        return _size.y;
    }

    void Window::set_width(const unsigned int width) {
        glfwSetWindowSize(_native_window, width, _size.y);
        _size.x = width;
    }

    void Window::set_height(const unsigned int height) {
        glfwSetWindowSize(_native_window, _size.x, height);
        _size.y = height;
    }

    // title
    char *Window::get_title(void) {
        return _title;
    }

    void Window::set_title(char *title) {
        glfwSetWindowTitle(_native_window, title);
        _title = title;
    }

    // icon
    void Window::set_icon(const char *path) {
        // TODO: Use image class lol
        GLFWimage icon;
        icon.pixels = stbi_load(path, &icon.width, &icon.height, nullptr, 4);
        glfwSetWindowIcon(_native_window, 1, &icon);
        stbi_image_free(icon.pixels);
    }

    // should_close
    bool Window::get_should_close(void) {
        return glfwWindowShouldClose(_native_window);
    }

    void Window::set_should_close(const bool should_close) {
        glfwSetWindowShouldClose(_native_window, should_close);
    }

    GLFWwindow *Window::get_native_window(void) {
        return _native_window;
    }
}