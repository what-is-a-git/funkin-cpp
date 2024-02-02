#include "core/ref_counted.h"

namespace crystal {
    RefCounted::RefCounted() {
        references = 0;
    }

    RefCounted::~RefCounted() {}

    void RefCounted::reference(void) {
        references++;
    }

    // WARNING: Doesn't delete self.
    void RefCounted::unreference(void) {
        references--;
    }
}
