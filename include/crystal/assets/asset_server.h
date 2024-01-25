#pragma once

#include <unordered_map>
#include <string>

#include "core/rendering/shader.h"
#include "core/rendering/texture.h"

#include "core/audio/audio_stream.h"

namespace crystal {
    class AssetServer {
        private:
            static std::unordered_map<std::string, Texture*> _texture_map;
            static std::unordered_map<std::string, Shader*> _shader_map;
            static std::unordered_map<std::string, AudioStream*> _audio_stream_map;
        public:
            AssetServer() = delete;

            static void init(void);
            static void dispose(void);

            static Texture *get_texture(const std::string path);
            static Texture *get_texture(const std::string path, const int filter_mode, const int wrap_mode);

            static void dispose_texture(const std::string key);
            static void check_textures(void);

            // Uses .frag and .vert file extensions.
            static Shader *get_shader(const std::string path);
            static Shader *get_shader(const std::string key, const char *fragment_path, const char *vertex_path);

            static void dispose_shader(const std::string key);

            static AudioStream *get_audio_stream(const std::string path);
            static void dispose_audio_stream(const std::string key);
    };
}