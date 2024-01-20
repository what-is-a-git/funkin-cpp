#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "core/color.h"

#include "assets/asset_server.h"

namespace crystal {
    class Sprite2D {
        private:
            glm::uvec2 _size;
            Texture *_texture;

            glm::mat4 _transform;
        public:
            bool active;

            glm::vec4 source_rect;

            glm::dvec2 position, scale;
            double rotation;

            glm::dvec2 origin;

            Color tint;
            Shader *shader;

            int texture_filter_mode, texture_wrap_mode;

            Sprite2D();
            Sprite2D(double x, double y);
            Sprite2D(double x, double y, Texture *texture);
            virtual ~Sprite2D();

            // No real use to this by default.
            virtual void step(double delta) {};
            virtual void draw(void);

            // Technically just a shortcut for setting texture_filter_mode
            // may be helpful in cases where including glad/glad.h isn't ideal or wanted.
            void set_antialiased(bool antialiased);

            void set_repeat(int repeat_mode);

            double get_rotation_degrees(void);
            void set_rotation_degrees(double rotation_degrees);

            glm::uvec2 get_size(void);

            // Only use if you know what you're doing.
            void set_size(glm::uvec2 size);

            // Lots of variables

            void load_texture(const std::string path);
            Texture *get_texture(void);
            void set_texture(Texture *texture);

            void load_shader(const std::string path);
    };
}
