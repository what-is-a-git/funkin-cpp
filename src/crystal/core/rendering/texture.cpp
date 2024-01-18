#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include "core/rendering/texture.h"

namespace crystal {
    Texture::Texture() {
        glGenTextures(1, &_native_texture);
    }

    Texture::~Texture() {
        glDeleteTextures(1, &_native_texture);
    }

    void Texture::bind(void) {
        glBindTexture(GL_TEXTURE_2D, _native_texture);
    }

    void Texture::generate_mipmaps(void) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    void Texture::load_texture(const unsigned int width, const unsigned int height, const void *data, const bool mipmaps) {
        bind();
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        _size = glm::uvec2(width, height);

        if (mipmaps) {
            generate_mipmaps();
        }
    }

    int Texture::get_wrap_mode(void) {
        int _wrap_mode;
        glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, &_wrap_mode);
        return _wrap_mode;
    }

    void Texture::set_wrap_mode(const int wrap_mode) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_mode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_mode);
    }

    int Texture::get_filter_mode(void) {
        int _filter_mode;
        glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, &_filter_mode);
        return _filter_mode;
    }

    void Texture::set_filter_mode(const int filter_mode) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_mode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_mode);
    }

    glm::uvec2 Texture::get_size(void) {
        return _size;
    }

    unsigned int Texture::get_width(void) {
        return _size.x;
    }

    unsigned int Texture::get_height(void) {
        return _size.y;
    }

    native_texture Texture::get_native(void) {
        return _native_texture;
    }

    Texture *Texture::get_from_file(const char *path, const int filter_mode, const int wrap_mode) {
        Texture *_return_texture = new Texture();

        int width, height;
        unsigned char *data = stbi_load(path, &width, &height, nullptr, 4);

        if (data) {
            _return_texture->load_texture(width, height, data, false);
            _return_texture->set_filter_mode(filter_mode);
            _return_texture->set_wrap_mode(wrap_mode);
            _return_texture->generate_mipmaps();
        } else {
            fprintf(stderr, "ERROR: Failed to load texture at path: %s\n", path);
        }

        stbi_image_free(data);

        return _return_texture;
    }
}