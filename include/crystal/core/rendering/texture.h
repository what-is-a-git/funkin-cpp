#pragma once

#include "glm/glm.hpp"

namespace crystal {
    typedef unsigned int native_texture;

    class Texture {
        private:
            native_texture _native_texture;
            glm::uvec2 _size;
        public:
            Texture();
            ~Texture();

            void bind(void);

            void load_texture(const unsigned int width, const unsigned int height, const void *data, const bool mipmaps);
            void generate_mipmaps(void);

            // Variables you get / set

            int get_wrap_mode(void);
            void set_wrap_mode(const int wrap_mode);

            int get_filter_mode(void);
            void set_filter_mode(const int filter_mode);

            glm::uvec2 get_size(void);
            unsigned int get_width(void);
            unsigned int get_height(void);

            native_texture get_native(void);

            static Texture *get_from_file(const char *path, const int filter_mode, const int wrap_mode);
    };
}