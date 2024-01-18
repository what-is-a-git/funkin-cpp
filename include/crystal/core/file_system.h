#pragma once

namespace crystal {
    class FileSystem {
        public:
            FileSystem() = delete;
            static char *get_file_contents(const char *path);
            static bool get_file_exists(const char *path);
    };
}