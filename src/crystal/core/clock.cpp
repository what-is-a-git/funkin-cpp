#include "core/clock.h"

namespace crystal {
    Clock::Clock() {
        time = 0.0;
        active = true;
    }

    void Clock::step(const double delta) {
        if (!active) {
            return;
        }

        time += delta;
    }
}