#include "core/ref_counted.h"

namespace crystal {
    RefCounted::RefCounted() {
        _references = 0;
    }

    void RefCounted::reference(void) {
        _references++;
    }

    // Automatically deletes self when no references are left.
    void RefCounted::unreference(void) {
        _references--;

        if (_references <= 0) {
            delete this;
        }
    }

    int RefCounted::get_references(void) {
        return _references;
    }
}