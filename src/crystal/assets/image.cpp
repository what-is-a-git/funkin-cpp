#include <stdio.h>
#include <cstring>

#include "assets/image.h"
#include "assets/png.h"

#include "core/string.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

namespace crystal {
    Image::Image() {
        size = glm::uvec2(0, 0);
        _data = NULL;
    }

    Image::~Image() {
        if (_data == NULL) {
            return;
        }

        free(_data);
    }

    unsigned char *Image::get_data(void) {
        return _data;
    }

    // uses libpng or similar backend as first pick if supported
    // stb image is the fallback because it's simpler BUT slower in most cases
    Image *Image::load_from_path(const char *path) {
        Image *image = new Image();
        char *extension = string_get_extension(path);

        if (strcmp(extension, "png") == 0) {
            image->_data = PNG::load_rgba8_from_path(path, &image->size);
        } else {
            int width, height;

            image->_data = stbi_load(path, &width, &height, NULL, 4);
            // we have to do this because size_t can't just be int ig :/
            image->size = glm::uvec2(width, height);
        }

        free(extension);
        return image;
    }
}