#include "core/timer.h"

namespace crystal {
    Timer::Timer() {
        _clock = Clock();
        time_left = 0.0;
        wait_time = 1.0;
        one_shot = true;
        active = true;
    }

    // By default the Timer class is one shot.
    Timer::Timer(const double _wait_time) {
        _clock = Clock();
        time_left = 0.0;
        wait_time = _wait_time;
        one_shot = true;
        active = true;
    }
    
    Timer::Timer(const double _wait_time, const bool _one_shot) {
        _clock = Clock();
        time_left = 0.0;
        wait_time = _wait_time;
        one_shot = _one_shot;
        active = true;
    }

    // The bool return can be used to determine whether or not the timer just finished.
    bool Timer::step(const double delta) {
        if (!active) {
            return false;
        }

        _clock.step(delta);
        time_left = _clock.time;

        if (time_left >= wait_time) {
            _clock.time = one_shot ? wait_time : 0.0;
            time_left = _clock.time;
            active = !one_shot;

            return true;
        }

        return false;
    }
}