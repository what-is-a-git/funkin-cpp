#pragma once

#include <glm/vec2.hpp>

#include "core/audio/audio_server.h"
#include "core/rendering/rendering_server.h"
#include "core/debug/debug_info.h"
#include "core/clock.h"
#include "core/color.h"
#include "core/math.h"

#include "scene/scene.h"
#include "scene/sprite.h"

#define GAME_SIZE crystal::Engine::size

#ifndef TARGET
#define TARGET OTHER
#endif

namespace crystal {
    enum BuildType {
        DEV = 0,
        STABLE = 1,
        OTHER = 2
    };

    enum TransitionState {
        NONE = 0,
        IN = 1,
        HOLD = 2,
        OUT = 3
    };

    typedef struct _init_arguments {
        glm::uvec2 size;
        char *name;
        unsigned int max_framerate; VSyncMode vsync_mode;
        Color default_clear_color;
        Scene *initial_scene;
    } init_arguments;

    class Engine {
        private:
            static init_arguments _init_arguments;
            static Scene *_current_scene;
            static unsigned int _max_framerate;

            static double _last_time;
            static double _elapsed;

            static Clock _draw_clock;
            static double _draw_delay;

            // Suggestion: Separate this into a different class? Maybe.
            // what-is-a-git - Dec 5, 2023
            static TransitionState _transition_state;
            static Clock _transition_clock;
            static Sprite *_transition_sprite;
            static Shader *_transition_shader;
            static Scene *_transition_scene;
        public:
            const static BuildType build_type = TARGET;
            static glm::uvec2 size;
            static DebugInfo *current_debug_info;

            Engine() = delete;
            
            static void init(const init_arguments init_arguments);
            static void step(void);
            static void step_transition(void);
            static void draw(void);

            static void dispose(void);

            static double get_time(void);
            static double get_elapsed(void);
            static bool get_should_close(void);

            static bool can_transition(void);
            static void switch_scene_to(Scene *new_scene);
            static TransitionState get_transition_state(void);

            static void callback_window_close(GLFWwindow *window);
    };
}