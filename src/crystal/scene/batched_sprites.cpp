#include "glad/glad.h"

#include "core/math.h"
#include "core/rendering/rendering_server.h"

#include "scene/batched_sprites.h"

namespace crystal {
    BatchedSprites::BatchedSprites(const BatchingMode batching_mode, const size_t sprite_buffer_count) {
        position = glm::dvec2(0.0, 0.0);
        scale = glm::dvec2(1.0, 1.0);
        tint = COLOR_WHITE;

        _mode = batching_mode;
        _vertex_max_count = sprite_buffer_count * 6;

        glGenVertexArrays(1, &_vertex_array_object);
        glBindVertexArray(_vertex_array_object);

        glGenBuffers(1, &_vertex_buffer_object);
        glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer_object);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * _vertex_max_count, NULL,
                       _mode == DYNAMIC ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        _texture = NULL;
        set_texture(AssetServer::get_texture("MISSING_TEXTURE"));

        shader = AssetServer::get_shader("BATCHED_SHADER");
        RenderingServer::use_shader(shader);
        shader->set_uniform_int("TEXTURE", 0);

        _vertices = (glm::vec4*) malloc(sizeof(glm::vec4) * _vertex_max_count);
        refresh(false);
    }

    BatchedSprites::~BatchedSprites() {
        glDeleteVertexArrays(1, &_vertex_array_object);
        glDeleteBuffers(1, &_vertex_buffer_object);

        free(_vertices);
    }

    void BatchedSprites::draw(void) {
        if (_mode == DYNAMIC) {
            refresh(false);
        }

        // quick and dirty fix for this, should work just fine for now :/
        // later fix: just add set_texture function lol
        RenderingServer::bind_texture(_texture);
        
        RenderingServer::use_shader(shader);
        shader->set_uniform_mat4("PROJECTION", RenderingServer::default_projection);

        _transform = glm::mat4(1.0f);
        _transform = glm::translate(_transform, glm::vec3(position, 0.0f));
        _transform = glm::scale(_transform, glm::vec3(scale, 1.0f));

        shader->set_uniform_mat4("TRANSFORM", _transform);
        shader->set_uniform_color("TINT", tint);

        glBindVertexArray(_vertex_array_object);
        glDrawArrays(GL_TRIANGLES, 0, _vertex_count);
    }

    void BatchedSprites::refresh(bool recreate_buffer) {
        _vertex_count = 0;
        glm::uvec2 _size = _texture->get_size();

        for (batched_sprite sprite : sprites) {
            glm::dvec2 size = (glm::dvec2)_size * sprite.scale;

            glm::mat4 transform = glm::mat4(1.0f);
            transform = glm::translate(transform, glm::vec3(sprite.position + (-sprite.origin * size), 0.0f));

            if (sprite.rotation != 0.0) {
                transform = glm::translate(transform, glm::vec3(sprite.origin.x * size.x, sprite.origin.y * size.y, 0.0f));
                transform = glm::rotate(transform, (float) sprite.rotation, glm::vec3(0.0f, 0.0f, 1.0f)); 
                transform = glm::translate(transform, glm::vec3(-sprite.origin.x * size.x, -sprite.origin.y * size.y, 0.0f));
            }

            transform = glm::scale(transform, glm::vec3(size, 1.0f));

            if (sprite.source_rect.z < 0.0f) {
                sprite.source_rect.z = _size.x;
            }

            if (sprite.source_rect.w < 0.0f) {
                sprite.source_rect.w = _size.y;
            }

            glm::vec4 source_rect = glm::vec4(sprite.source_rect.x / _size.x, sprite.source_rect.y / _size.y,
                    (sprite.source_rect.x + sprite.source_rect.z) / _size.x, (sprite.source_rect.y + sprite.source_rect.w) / _size.y);

            glm::vec4 vertex[6] = {
                // vertex 1 (top left triangle)
                glm::vec4(0.0f, 1.0f, source_rect.x, source_rect.w), // top left
                glm::vec4(0.0f, 0.0f, source_rect.x, source_rect.y), // bottom left
                glm::vec4(1.0f, 1.0f, source_rect.z, source_rect.w), // top right

                // vertex 2 (bottom right triangle)
                glm::vec4(0.0f, 0.0f, source_rect.x, source_rect.y), // bottom left
                glm::vec4(1.0f, 0.0f, source_rect.z, source_rect.y), // bottom right
                glm::vec4(1.0f, 1.0f, source_rect.z, source_rect.w) // top right
            };

            if (_vertex_count + 6 >= _vertex_max_count - 1 && sprites.end().base() != &sprite) {
                _vertex_count = 0;
                _vertex_max_count *= 2;

                free(_vertices);
                _vertices = (glm::vec4*) malloc(sizeof(glm::vec4) * _vertex_max_count);
                refresh(true);
                return;
            }

            for (int i = 0; i < 6; i++) {
                // we don't use this directly cuz it messes with uvs lol
                glm::vec4 transformed_vertex = transform * glm::vec4(vertex[i].x, vertex[i].y, 0.0f, 1.0f);

                glm::vec2 source_rect = glm::vec2(vertex[i].z, vertex[i].w);
                vertex[i] = glm::vec4(transformed_vertex.x, transformed_vertex.y, source_rect);

                _vertices[_vertex_count + i] = vertex[i];
            }

            _vertex_count += 6;
        }

        if (_vertex_count == 0) {
            return;
        }

        glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer_object);

        if (recreate_buffer) {
            glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * _vertex_max_count, NULL,
                       _mode == DYNAMIC ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
        }

        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec4) * _vertex_count,
                        _vertices);
    }

    void BatchedSprites::set_texture(Texture *texture) {
        if (_texture != NULL) {
            _texture->unreference();
        }

        if (texture == NULL) {
            _texture = NULL;
            return;
        }

        _texture = texture;
        _texture->reference();
    }

    Texture *BatchedSprites::get_texture(void) {
        return _texture;
    }

    void BatchedSprites::add_batched_sprite(batched_sprite sprite) {
        sprites.push_back(sprite);
    }

    BatchingMode BatchedSprites::get_batching_mode(void) {
        return _mode;
    }
}