#include <stdio.h>

#include "funkin/debug/console_debug_info.h"

namespace funkin {
    ConsoleDebugInfo::ConsoleDebugInfo() {
        _update_timer = new crystal::Timer(1.0, false);
        _frames_counted = 0;
    }

    ConsoleDebugInfo::~ConsoleDebugInfo() {
        _update_timer->~Timer();
    }

    void ConsoleDebugInfo::step(const double delta) {
        _frames_counted += 1;

        if (_update_timer->step(delta)) {
            printf("== Debug Info ==\n   %d FPS\n", _frames_counted);
            _frames_counted = 0;
        }
    }
}