#pragma once

namespace crystal {
    class RefCounted {
        private:
            int _references;
        public:
            RefCounted();
            ~RefCounted() = default;

            void reference(void);

            // Automatically deletes self when no references are left.
            void unreference(void);

            int get_references(void);
    };
}

