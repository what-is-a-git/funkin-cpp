#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "assets/asset_server.h"

namespace crystal {
    typedef struct {
        glm::vec4 source_rect;
        glm::dvec2 position, scale, origin;
        double rotation;
        void *meta;
    } batched_sprite;

    enum BatchingMode {
        BAKED = 0,
        DYNAMIC = 1
    };

    class BatchedSprites {
        private:
            BatchingMode _mode;

            int _texture_filter_mode, _texture_wrap_mode;

            glm::mat4 _transform;

            unsigned int _vertex_array_object, _vertex_buffer_object;
            size_t _vertex_capacity, _vertex_count, _vertex_max_count;
            glm::vec4 *_vertices;

            Texture *_texture;
        public:
            glm::dvec2 position;
            glm::dvec2 scale;

            Color tint;
            Shader *shader;

            std::vector<batched_sprite> sprites;

            BatchedSprites(const BatchingMode batching_mode, const size_t sprite_buffer_count);
            virtual ~BatchedSprites();

            virtual void step(double delta) {};
            virtual void draw(void);

            virtual void refresh(bool recreate_buffer);

            void set_texture(Texture *texture);
            Texture *get_texture(void);

            void add_batched_sprite(batched_sprite sprite);

            BatchingMode get_batching_mode(void);
    };
}