#include <string.h>

#include "core/engine.h"
#include "core/input/input_server.h"
#include "assets/asset_server.h"

namespace crystal {
    const BuildType Engine::build_type;

    init_arguments Engine::_init_arguments;
    Scene *Engine::_current_scene;
    unsigned int Engine::_max_framerate;

    double Engine::_last_time;
    double Engine::_elapsed;

    Clock Engine::_draw_clock;
    double Engine::_draw_delay;

    // Suggestion: Separate this into a different class? Maybe.
    // what-is-a-git - Dec 5, 2023
    TransitionState Engine::_transition_state;
    Clock Engine::_transition_clock;
    Sprite2D *Engine::_transition_sprite;
    Shader *Engine::_transition_shader;
    Scene *Engine::_transition_scene;

    DebugInfo *Engine::current_debug_info;
    glm::uvec2 Engine::size;

    void Engine::init(const init_arguments init_arguments) {
        _init_arguments = init_arguments;
        _max_framerate = _init_arguments.max_framerate;
        size = _init_arguments.size;

        RenderingServer::init();
        RenderingServer::create_primary_window(_init_arguments.size, _init_arguments.name);
        RenderingServer::set_clear_color(_init_arguments.default_clear_color);
        RenderingServer::set_vsync_mode(_init_arguments.vsync_mode);

        switch (_init_arguments.vsync_mode) {
            case OFF:
                if (_max_framerate > 0.0) {
                    _draw_delay = 1.0 / _max_framerate;
                } else {
                    _draw_delay = -1.0;
                }

                break;
            case DRAW:
                _draw_delay = 1.0 / RenderingServer::get_monitor_refresh_rate();
                break;
            default:
                _draw_delay = -1.0;
                break;
        }

        AudioServer::init();
        AssetServer::init();
        InputServer::init();

        // This is a sprite because in the future we might want to do more with transitions
        // than just a simple slide. Thanks! <3
        _transition_sprite = new Sprite2D(0.0, 0.0, nullptr);
        _transition_sprite->set_size(GAME_SIZE);
        _transition_sprite->origin = glm::dvec2(0.0);
        _transition_sprite->shader = AssetServer::get_shader("TRANSITION_SHADER");
        _transition_sprite->tint = COLOR_BLACK;
        
        _transition_shader = _transition_sprite->shader;
        RenderingServer::use_shader(_transition_shader);
        _transition_shader->set_uniform_float("VALUE", 0.0f);
        _transition_shader->set_uniform_int("RIGHT", 1);

        _transition_clock = Clock();
        _transition_state = NONE;

        _current_scene = _init_arguments.initial_scene;
        _current_scene->init();
        _last_time = get_time();
    }

    void Engine::step(void) {
        _elapsed = get_time() - _last_time;
        _last_time = get_time();

        glfwPollEvents();

        if (_draw_delay != -1.0) {
            _draw_clock.step(_elapsed);
        }

        if (current_debug_info != nullptr) {
            current_debug_info->step(_elapsed);
        }

        step_transition();

        if (_transition_state == IN || _transition_state == HOLD) {
            return;
        }

        _current_scene->step(_elapsed);

        InputServer::step();
    }

    void Engine::step_transition(void) {
        if (_transition_state == NONE) {
            return;
        }

        if (_transition_clock.time >= 0.5 && _transition_state == IN) {
            _transition_state = HOLD;

            _current_scene->init();
            _transition_scene->~Scene();

            // We have to use it here because _current_scene->init() can cause a shader state change.
            _transition_shader->use();
            _transition_shader->set_uniform_int("RIGHT", 0);
            _transition_state = OUT;
        }

        if (_transition_clock.time >= 1.0 && _transition_state == OUT) {
            _transition_state = NONE;
            _transition_clock.time = 0.0;
        }

        _transition_clock.step(_elapsed);

        if (_transition_state == IN) {
            _transition_shader->set_uniform_float("VALUE", ease_in_sine(_transition_clock.time * 2.0));
        } else {
            _transition_shader->set_uniform_float("VALUE", ease_out_sine(1.0 + ((_transition_clock.time * 2.0) - 1.0)));
        }
    }

    void Engine::draw(void) {
        if (_draw_delay != -1.0) {
            if (_draw_clock.time < _draw_delay) {
                return;
            } else {
                _draw_clock.time = 0.0;
            }
        }

        RenderingServer::clear();

        if (_transition_state == NONE) {
            _current_scene->draw();
        } else {
            if (_transition_state == IN) {
                _transition_scene->draw();
            } else if (_transition_state == OUT) {
                _current_scene->draw();
            }

            _transition_sprite->draw();
        }

        RenderingServer::present();
    }

    void Engine::dispose(void) {
        InputServer::dispose();
        AssetServer::dispose();
        RenderingServer::dispose();
        AudioServer::dispose();

        if (current_debug_info != nullptr) {
            delete current_debug_info;
        }
    }

    double Engine::get_time(void) {
        return glfwGetTime();
    }

    double Engine::get_elapsed(void) {
        return _elapsed;
    }

    bool Engine::get_should_close(void) {
        return RenderingServer::get_primary_window()->get_should_close();
    }

    void Engine::switch_scene_to(Scene *new_scene) {
        _transition_state = IN;
        _transition_scene = _current_scene;
        _current_scene = new_scene;
    }

    void Engine::callback_window_close(GLFWwindow *window) {
        glfwSetWindowShouldClose(window, true);
        // RenderingServer::get_primary_window()->set_should_close(true);
    }
}