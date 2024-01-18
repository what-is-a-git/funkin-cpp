#pragma once

#include <unordered_map>

#include <glfw/glfw3.h>

namespace crystal {
    enum InputState {
        JUST_RELEASED = 0,
        RELEASED = 1,
        PRESSING = 2,
        JUST_PRESSED = 3,
        REPEATING = 4
    };

    class InputServer {
        private:
            static void _native_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
        public:
            // Possible to use but not recommended, using provided abstractions is safer.
            // (This hashmap only contains keys that have been recorded during the application's lifetime).
            static std::unordered_map<int, InputState> input_states;

            InputServer() = delete;

            static void init(void);
            static void dispose(void);

            // Used for JUST_PRESSED and JUST_RELEASED primarily.
            static void step(void);

            static InputState get_key_state(int key);

            static bool is_key_pressed(int key);
            static bool is_key_pressed(int key, bool include_repeating);
    };
}