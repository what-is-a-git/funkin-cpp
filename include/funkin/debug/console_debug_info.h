#pragma once

#include "core/timer.h"
#include "core/debug/debug_info.h"

namespace funkin {
    class ConsoleDebugInfo : public crystal::DebugInfo {
        private:
            crystal::Timer *_update_timer;
            unsigned int _frames_counted;
        public:
            ConsoleDebugInfo();
            virtual ~ConsoleDebugInfo();

            virtual void step(const double delta);
    };
}