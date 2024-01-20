#pragma once

#include <stdlib.h>

namespace crystal {
    typedef struct {
        unsigned char *data;
        size_t size;
    } file_binary;

    class FileSystem {
        public:
            FileSystem() = delete;

            static char *get_file_contents(const char *path);
            static file_binary get_file_binary(const char *path);

            static bool get_file_exists(const char *path);
    };
}