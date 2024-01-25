#include <glad/glad.h>

#include "stb/stb_image.h"

#include "assets/asset_server.h"

namespace crystal {
    std::unordered_map<std::string, Texture*> AssetServer::_texture_map;
    std::unordered_map<std::string, Shader*> AssetServer::_shader_map;
    std::unordered_map<std::string, AudioStream*> AssetServer::_audio_stream_map;

    void AssetServer::init(void) {
        // Preload assets

        // Textures
        Texture *missing_texture = Texture::get_from_file("assets/crystal/images/missing_texture.png", GL_LINEAR, GL_REPEAT);
        missing_texture->reference();

        _texture_map.insert({ "MISSING_TEXTURE", missing_texture });

        // Shaders
        AssetServer::get_shader("BASIC_SHADER", "assets/shaders/basic.frag", "assets/shaders/basic.vert"); // aka COLOR_RECT_SHADER
        AssetServer::get_shader("BATCHED_SHADER", "assets/shaders/sprite_2d.frag", "assets/shaders/basic.vert");
        AssetServer::get_shader("SPRITE_2D_SHADER", "assets/shaders/sprite_2d.frag", "assets/shaders/sprite_2d.vert");
        AssetServer::get_shader("TRANSITION_SHADER", "assets/shaders/transition.frag", "assets/shaders/basic.vert");

        // Sounds (not even preloadable rn)
    }

    void AssetServer::dispose(void) {
        while (!_texture_map.empty()) {
            dispose_texture(_texture_map.begin()->first);
        }

        while (!_shader_map.empty()) {
            dispose_shader(_shader_map.begin()->first);
        }

        while (!_audio_stream_map.empty()) {
            dispose_audio_stream(_audio_stream_map.begin()->first);
        }
    }

    Texture *AssetServer::get_texture(const std::string path) {
        if (_texture_map.count(path) == 0) {
            Texture *_texture = Texture::get_from_file(path.c_str(), GL_LINEAR, GL_REPEAT);
            _texture_map.insert({ path, _texture });
        }

        return _texture_map[path];
    }

    Texture *AssetServer::get_texture(const std::string path, const int filter_mode, const int wrap_mode) {
        if (_texture_map.count(path) == 0) {
            Texture *_texture = Texture::get_from_file(path.c_str(), filter_mode, wrap_mode);
            _texture_map.insert({ path, _texture });
        }

        return _texture_map[path];
    }

    void AssetServer::dispose_texture(const std::string key) {
        if (_texture_map.count(key) == 1) {
            Texture *_texture = _texture_map[key];
            delete _texture;
            _texture_map.erase(key);
        }
    }

    void AssetServer::check_textures(void) {
        auto iterator = _texture_map.begin();

        while (iterator != _texture_map.end()) {
            std::pair<std::string, Texture*> pair = *iterator;
            Texture *texture = pair.second;
            // printf("{ %s: %ld }\n", pair.first.c_str(), pair.second->references);

            if (texture->references <= 0) {
                if (texture->references < 0) {
                    printf("WARNING: Texture has a reference count lower than 0, there is probably a referencing issue somewhere.\n");
                }

                iterator = _texture_map.erase(iterator);
                delete texture;
                continue;
            }

            iterator++;
        }
    }

    // Uses .frag and .vert file extensions.
    Shader *AssetServer::get_shader(const std::string path) {
        if (_shader_map.count(path) == 0) {
            Shader *_shader = Shader::get_from_file(path.c_str());
            _shader_map.insert({ path, _shader });
        }

        return _shader_map[path];
    }

    Shader *AssetServer::get_shader(const std::string key, const char *fragment_path, const char *vertex_path) {
        if (_shader_map.count(key) == 0) {
            Shader *_shader = Shader::get_from_file(fragment_path, vertex_path);
            _shader_map.insert({ key, _shader });
        }

        return _shader_map[key];
    }

    void AssetServer::dispose_shader(const std::string key) {
        if (_shader_map.count(key) == 1) {
            Shader *_shader = _shader_map[key];
            delete _shader;
            _shader_map.erase(key);
        }
    }

    AudioStream *AssetServer::get_audio_stream(const std::string path) {
        return new AudioStream(path.c_str());
        // if (_audio_stream_map.find(path) == _audio_stream_map.end()) {
        //     AudioStream *_stream = new AudioStream(path.c_str());
        //     _audio_stream_map.insert({ path, _stream });
        // }

        // return _audio_stream_map[path];
    }

    void AssetServer::dispose_audio_stream(const std::string key) {
        if (_audio_stream_map.count(key) == 1) {
            AudioStream *_stream = _audio_stream_map[key];
            delete _stream;
            _audio_stream_map.erase(key);
        }
    }
}