#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "core/rendering/shader.h"
#include "core/color.h"

namespace crystal {
    class ColorRect {
        private:
            // Private because it shouldn't be changed, unlike a sprite which is more flexible.
            Shader *_shader;
            glm::mat4 _transform;
        public:
            Color color;

            glm::dvec2 position;
            glm::dvec2 size;
            double rotation;

            glm::dvec2 origin;

            ColorRect();
            ColorRect(double x, double y, double width, double height, Color color);
            ~ColorRect() = default;

            void draw(void);

            double get_rotation_degrees(void);
            void set_rotation_degrees(double rotation_degrees);
    };
}