#pragma once

#include <stdlib.h>

namespace crystal {
    class RefCounted {
        public:
            size_t references;

            RefCounted();
            ~RefCounted();

            void reference(void);

            // WARNING: Doesn't delete self.
            void unreference(void);
    };
}

