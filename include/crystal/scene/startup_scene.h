#pragma once

#include "scene/scene.h"
#include "scene/sprite_2d.h"
#include "core/clock.h"

namespace crystal {
    class StartupScene : public Scene {
        private:
            Sprite2D _logo;
            Sprite2D _logo_text;
            Clock _clock;
        public:
            Scene *_next_scene;

            StartupScene();
            StartupScene(Scene *next_scene);
            virtual ~StartupScene();

            virtual void init(void);
            virtual void step(const double delta);
            virtual void draw(void);
    };
}