#include "core/engine.h"
#include "core/math.h"
#include "scene/startup_scene.h"

namespace crystal {
    StartupScene::StartupScene() {
        _next_scene = nullptr;
    }

    StartupScene::StartupScene(Scene *next_scene) {
        _next_scene = next_scene;
    }

    StartupScene::~StartupScene() {}

    void StartupScene::init(void) {
        _logo = Sprite2D(GAME_SIZE.x * 0.5, GAME_SIZE.y * 0.5, AssetServer::get_texture("assets/crystal/images/logo.png"));
        _logo.scale.x = 0.0;
        _logo.scale.y = 0.0;

        _logo_text = Sprite2D(GAME_SIZE.x * 0.5, GAME_SIZE.y * 0.5, AssetServer::get_texture("assets/crystal/images/logo_text.png"));
        _logo_text.scale.x = 0.0;
        _logo_text.scale.y = 0.0;

        _clock = Clock();
    }

    void StartupScene::step(const double delta) {
        if (!_active) {
            return;
        }

        _logo.step(delta);
        _logo_text.step(delta);
        _clock.step(delta);

        // TODO: Refactor this into a tween / animation system :3

        if (_clock.time >= 1.5) {
            double _alpha = 1.0 - ease_out_sine(fmin(_clock.time - 1.5, 1.0));
            _logo.tint.a = _alpha;
            _logo_text.tint.a = _alpha;

            if (_alpha <= 0.0) {
                _logo.~Sprite2D();
                _logo_text.~Sprite2D();
                AssetServer::dispose_texture("assets/crystal/images/logo.png");
                AssetServer::dispose_texture("assets/crystal/images/logo_text.png");
                _active = false;

                Engine::switch_scene_to(_next_scene);

                return;
            }
        }

        // Potential optimization I suppose?
        if (_clock.time > 1.0) {
            return;
        }

        // Logo rotation
        _logo.set_rotation_degrees(lerp(22.5, 0.0, ease_out_sine(_clock.time)));

        if (_clock.time > 0.8) {
            return;
        }

        // Logo scale
        _logo.scale.x = lerp(0.0, 0.5, ease_out_sine(_clock.time / 0.8));
        _logo.scale.y = _logo.scale.x;

        if (_clock.time > 0.7) {
            return;
        }

        // Text scale
        if (_clock.time <= 0.3) {
            _logo_text.scale.x = lerp(0.0, 0.51, ease_in_sine(_clock.time / 0.3));
        } else {
            _logo_text.scale.x = lerp(0.51, 0.5, ease_out_sine((_clock.time - 0.3) / 0.4));
        }

        _logo_text.scale.y = _logo_text.scale.x;
    }

    void StartupScene::draw(void) {
        _logo.draw();
        _logo_text.draw();
    }
}