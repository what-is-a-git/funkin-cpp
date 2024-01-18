#pragma once

#include "core/clock.h"

namespace crystal {
    class Timer {
        private:
            Clock _clock;
        public:
            double time_left;
            double wait_time;
            bool one_shot;
            bool active;

            // By default the Timer class is one shot.
            Timer();
            Timer(const double _wait_time);
            Timer(const double _wait_time, const bool _one_shot);
            ~Timer() = default;

            // The bool return can be used to determine whether or not the timer just finished.
            bool step(const double delta);
    };
}