#pragma once

namespace crystal {
    // An abstract class with no real methods that represents a scene in-game.
    class Scene {
        protected:
            bool _active;
        public:
            Scene();
            virtual ~Scene();

            virtual void init(void);
            virtual void step(const double delta);
            virtual void draw(void);

            virtual void on_switch_dispose(void);
    };
}