#include <glad/glad.h>

#include "core/engine.h"
#include "scene/sprite_2d.h"

namespace crystal {
    Sprite2D::~Sprite2D() {
        if (_texture == NULL) {
            return;
        }
        
        _texture->unreference();
    }

    Sprite2D::Sprite2D(double x, double y) {
        active = true;
        position = glm::dvec2(x, y);
        scale = glm::dvec2(1.0, 1.0);
        rotation = 0.0;
        origin = glm::dvec2(0.5, 0.5);
        tint = COLOR_WHITE;
        source_rect = glm::vec4(0.0f, 0.0f, -1.0f, -1.0f);

        _texture = NULL;
        shader = NULL;

        load_texture("MISSING_TEXTURE");
        load_shader("SPRITE_2D_SHADER");
    }

    Sprite2D::Sprite2D(double x, double y, Texture *texture) {
        active = true;
        position = glm::dvec2(x, y);
        scale = glm::dvec2(1.0, 1.0);
        rotation = 0.0;
        origin = glm::dvec2(0.5, 0.5);
        tint = COLOR_WHITE;
        source_rect = glm::vec4(0.0f, 0.0f, -1.0f, -1.0f);

        _texture = NULL;
        shader = NULL;

        set_texture(texture);
        load_shader("SPRITE_2D_SHADER");
    }

    void Sprite2D::draw(void) {
        RenderingServer::bind_texture(_texture);
        
        shader->use();
        shader->set_uniform_mat4("PROJECTION", RenderingServer::default_projection);

        if (source_rect.z < 0.0f) {
            source_rect.z = _size.x;
        }

        if (source_rect.w < 0.0f) {
            source_rect.w = _size.y;
        }

        glm::dvec2 size = glm::dvec2(source_rect.z, source_rect.w) * scale;

        _transform = glm::mat4(1.0f);
        _transform = glm::translate(_transform, glm::vec3(position + (-origin * size), 0.0f));

        if (rotation != 0.0) {
            _transform = glm::translate(_transform, glm::vec3(origin.x * size.x, origin.y * size.y, 0.0f));
            _transform = glm::rotate(_transform, (float) rotation, glm::vec3(0.0f, 0.0f, 1.0f)); 
            _transform = glm::translate(_transform, glm::vec3(-origin.x * size.x, -origin.y * size.y, 0.0f));
        }

        _transform = glm::scale(_transform, glm::vec3(size, 1.0f));
        shader->set_uniform_mat4("TRANSFORM", _transform);
        shader->set_uniform_color("TINT", tint);

        glm::vec4 _source_rect = glm::vec4(source_rect.x / _size.x, source_rect.y / _size.y,
                (source_rect.x + source_rect.z) / _size.x, (source_rect.y + source_rect.w) / _size.y);

        shader->set_uniform_vec4("SOURCE_RECT", _source_rect);
        shader->set_uniform_int("TEXTURE", 0);

        RenderingServer::draw_quad();
    }

    // Technically just a shortcut for setting texture_filter_mode
    // may be helpful in cases where including glad/glad.h isn't ideal or wanted.
    void Sprite2D::set_antialiased(bool antialiased) {
        texture_filter_mode = antialiased ? GL_LINEAR : GL_NEAREST;
        _texture->set_filter_mode(texture_filter_mode);
    }

    void Sprite2D::set_repeat(int repeat_mode) {
        texture_wrap_mode = repeat_mode;
        _texture->set_wrap_mode(texture_wrap_mode);
    }

    double Sprite2D::get_rotation_degrees(void) {
        return glm::degrees(rotation);
    }

    void Sprite2D::set_rotation_degrees(double rotation_degrees) {
        rotation = glm::radians(rotation_degrees);
    }

    glm::uvec2 Sprite2D::get_size(void) {
        return _size;
    }

    // Only use if you know what you're doing.
    void Sprite2D::set_size(glm::uvec2 size) {
        _size = size;
    }

    // Lots of variables
    
    void Sprite2D::load_texture(const std::string path) {
        set_texture(AssetServer::get_texture(path));
    }

    Texture *Sprite2D::get_texture(void) {
        return _texture;
    }

    void Sprite2D::set_texture(Texture *texture) {
        if (_texture != NULL) {
            _texture->unreference();
        }

        if (texture == NULL) {
            _texture = NULL;
            return;
        }

        _texture = texture;
        _texture->reference();
        _size = _texture->get_size();
    }

    void Sprite2D::load_shader(const std::string path) {
        shader = AssetServer::get_shader(path);
    }
}