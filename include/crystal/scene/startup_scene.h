#pragma once

#include "scene/scene.h"
#include "scene/sprite.h"
#include "core/clock.h"

namespace crystal {
    class StartupScene : public Scene {
        private:
            Sprite *_logo;
            Sprite *_logo_text;
            Clock _clock;
        public:
            Scene *_next_scene;

            StartupScene();
            StartupScene(Scene *next_scene);
            virtual ~StartupScene();

            virtual void init(void);
            virtual void step(const double delta);
            virtual void draw(void);

            virtual void on_switch_dispose(void) {};
    };
}