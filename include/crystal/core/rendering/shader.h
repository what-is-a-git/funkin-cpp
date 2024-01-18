#pragma once

#include "glm/glm.hpp"

#include "core/color.h"

namespace crystal {
    typedef unsigned int native_shader;

    class Shader {
        private:
            native_shader _native_shader;
        public:
            Shader(const char *fragment_source, const char *vertex_source);
            ~Shader();

            void use(void);

            int get_uniform_int(const char *key);
            void set_uniform_int(const char *key, int value);

            float get_uniform_float(const char *key);
            void set_uniform_float(const char *key, float value);

            void set_uniform_color(const char *key, Color value);
            void set_uniform_mat4(const char *key, glm::mat4 value);

            void set_uniform_vec4(const char *key, glm::vec4 value);

            native_shader get_native(void);

            // Will use one file name with the extensions of .frag and .vert.
            static Shader *get_from_file(const char *shader_path);
            static Shader *get_from_file(const char *fragment_path, const char *vertex_path);
    };
}