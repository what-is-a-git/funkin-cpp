#pragma once

#include <stdlib.h>
#include <glm/glm.hpp>

namespace crystal {
    class Image {
        private:
            unsigned char *_data;
        public:
            glm::uvec2 size;

            Image();
            ~Image();

            unsigned char *get_data(void);

            // uses libpng or similar backend as first pick if supported
            // stb image is the fallback because it's simpler BUT slower in most cases
            static Image *load_from_path(const char *path);
    };
}
