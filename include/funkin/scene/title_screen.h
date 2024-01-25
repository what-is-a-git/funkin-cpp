#pragma once

#include "scene/scene.h"
#include "scene/sprite_2d.h"

namespace funkin {
    class TitleScreen : public crystal::Scene {
        public:
            crystal::Sprite2D *girlfriend;

            TitleScreen();
            virtual ~TitleScreen();

            virtual void init(void);
            virtual void step(const double delta);
            virtual void draw(void);

            virtual void on_switch_dispose(void);
    };
}