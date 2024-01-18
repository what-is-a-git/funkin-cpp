#pragma once

namespace crystal {
    class DebugInfo {
        public:
            DebugInfo();
            virtual ~DebugInfo() = default;
            virtual void step(const double delta);
    };
}