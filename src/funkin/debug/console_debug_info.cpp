#include <stdio.h>

#include "funkin/debug/console_debug_info.h"
#include "vendor/memory_usage.h"

namespace funkin {
    ConsoleDebugInfo::ConsoleDebugInfo() {
        _update_timer = new crystal::Timer(1.0, false);
        _frames_counted = 0;
    }

    ConsoleDebugInfo::~ConsoleDebugInfo() {
        delete _update_timer;
    }

    void ConsoleDebugInfo::step(const double delta) {
        _frames_counted += 1;

        if (_update_timer->step(delta)) {
            printf("== Debug Info ==\n %d FPS -- %f mb\n", _frames_counted, getCurrentRSS() / 1024.0 / 1024.0);
            _frames_counted = 0;
        }
    }
}