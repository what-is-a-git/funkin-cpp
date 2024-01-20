#pragma once

#include "scene/scene.h"

namespace funkin {
    class TitleScreen : public crystal::Scene {
        public:
            TitleScreen();
            virtual ~TitleScreen();

            virtual void init(void);
            virtual void step(const double delta);
            virtual void draw(void);
    };
}