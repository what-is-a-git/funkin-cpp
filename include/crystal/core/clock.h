#pragma once

namespace crystal {
    class Clock {
        public:
            double time;
            bool active;

            Clock();
            ~Clock() = default;

            void step(const double delta);
    };
}