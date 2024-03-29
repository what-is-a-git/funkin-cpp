#pragma once

#include "scene/scene.h"
#include "scene/sprite.h"
#include "scene/batched_sprites.h"

namespace funkin {
    class TitleScreen : public crystal::Scene {
        public:
            crystal::Sprite *girlfriend;

            TitleScreen();
            virtual ~TitleScreen();

            virtual void init(void);
            virtual void step(const double delta);
            virtual void draw(void);

            virtual void on_switch_dispose(void);
    };
}