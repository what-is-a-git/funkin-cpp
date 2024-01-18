#include "scene/color_rect.h"

#include "core/rendering/rendering_server.h"
#include "assets/asset_server.h"

namespace crystal {
    // Don't use this blank constructor. Will have issues. :)
    ColorRect::ColorRect() {}

    ColorRect::ColorRect(double x, double y, double width, double height, Color color) {
        position = glm::dvec2(x, y);
        size = glm::dvec2(width, height);
        rotation = 0.0;
        origin = glm::dvec2(0.5, 0.5);
        this->color = color;

        _shader = AssetServer::get_shader("BASIC_SHADER");
        RenderingServer::use_shader(_shader);
    }

    void ColorRect::draw(void) {
        RenderingServer::use_shader(_shader);
        _shader->set_uniform_mat4("PROJECTION", RenderingServer::default_projection);

        _transform = glm::mat4(1.0f);
        _transform = glm::translate(_transform, glm::vec3(position + (-origin * size), 0.0f));

        if (rotation != 0.0) {
            _transform = glm::translate(_transform, glm::vec3(origin.x * size.x, origin.y * size.y, 0.0f));
            _transform = glm::rotate(_transform, (float) rotation, glm::vec3(0.0f, 0.0f, 1.0f)); 
            _transform = glm::translate(_transform, glm::vec3(-origin.x * size.x, -origin.y * size.y, 0.0f));
        }

        _transform = glm::scale(_transform, glm::vec3(size, 1.0f));

        _shader->set_uniform_mat4("TRANSFORM", _transform);
        _shader->set_uniform_color("TINT", color);

        RenderingServer::draw_quad();
    }

    double ColorRect::get_rotation_degrees(void) {
        return glm::degrees(rotation);
    }

    void ColorRect::set_rotation_degrees(double rotation_degrees) {
        rotation = glm::radians(rotation_degrees);
    }
}