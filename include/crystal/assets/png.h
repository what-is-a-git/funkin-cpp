#pragma once

#include <glm/glm.hpp>

#include <png/png.h>
#include <stdlib.h>

namespace crystal {
    typedef struct _png_io {
        unsigned char *data;
        size_t bytes_left;
    } png_io;

    class PNG {
        public:
            PNG() = delete;

            static void libpng_read(png_structp png_ptr, png_bytep data, png_size_t length);
            static unsigned char *load_rgba8_from_path(const char *path, glm::uvec2 *size);
    };
}