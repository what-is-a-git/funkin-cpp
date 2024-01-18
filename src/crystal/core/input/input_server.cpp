#include "core/input/input_server.h"

#include "core/rendering/rendering_server.h"

#include <stdio.h>

namespace crystal {
    std::unordered_map<int, InputState> InputServer::input_states;

    void InputServer::init(void) {
        glfwSetKeyCallback(RenderingServer::get_primary_window()->get_native_window(), _native_key_callback);
    }

    void InputServer::dispose(void) {
        glfwSetKeyCallback(RenderingServer::get_primary_window()->get_native_window(), NULL);
    }

    void InputServer::step(void) {
        for (std::pair<const int, InputState> keypair : input_states) {
            int key = keypair.first;
            InputState state = keypair.second;

            if (state == JUST_PRESSED) {
                input_states[key] = PRESSING;
                continue;
            }

            if (state == JUST_RELEASED) {
                input_states[key] = RELEASED;
                continue;
            }
        }
    }

    void InputServer::_native_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
        switch (action) {
            case GLFW_PRESS:
                input_states[key] = JUST_PRESSED;
                break;
            case GLFW_RELEASE:
                input_states[key] = JUST_RELEASED;
                break;
            case GLFW_REPEAT:
                input_states[key] = REPEATING;
                break;
        }
    }

    InputState InputServer::get_key_state(int key) {
        if (input_states.count(key) == 0) {
            input_states[key] = RELEASED;
        }

        return input_states[key];
    }

    bool InputServer::is_key_pressed(int key) {
        // 0 = JUST_RELEASED, 1 = RELEASED, anything higher is pressing.
        return get_key_state(key) > 1;
    }

    bool InputServer::is_key_pressed(int key, bool include_repeating) {
        // Bit messy but it works.
        return include_repeating ? (is_key_pressed(key)) : (get_key_state(key) == JUST_PRESSED || get_key_state(key) == PRESSING);
    }
}