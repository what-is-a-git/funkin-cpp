#include <stdlib.h>
#include <cstring>
#include "glad/glad.h"

#include "core/engine.h"
#include "core/input/input_server.h"
#include "assets/asset_server.h"
#include "core/rendering/rendering_server.h"
#include "core/rendering/shader.h"
#include "funkin/scene/title_screen.h"

namespace funkin {
    typedef struct { float cols[4][4]; } Matrix;
    typedef struct { float position[2]; float texcoord[2]; } Vertex;

    unsigned int vao;
    unsigned int vbo;

    const char *vertex ="#version 330 core\n"
                        "layout(location=0) in vec2 a_position;\n"
                        "layout(location=1) in vec2 a_texindex;\n"
                        "out vec2 v_texindex;\n"
                        "uniform mat4 u_mvp;\n"
                        "void main() {\n"
                        "    gl_Position = u_mvp * vec4(a_position, 0.0, 1.0);\n"
                        "    v_texindex = a_texindex;\n"
                        "}\n";

    const char *fragment = "#version 330 core\n"
                            "in vec2 v_texindex;\n"
                            "out vec4 f_color;\n"
                            "uniform sampler2D u_texture;\n"
                            "void main() {\n"
                            "    f_color = texture(u_texture, v_texindex);\n"
                            "}\n";

    crystal::Texture *balls;
    crystal::Shader *cool;
    const int vertex_capacity = 100000 * 6;
    int vertex_count = 0;
    Vertex *vertices;
    Matrix mvp;

    void r_flush(void) {
      if (vertex_count == 0) {
        return;
      }

      cool->use();

      glActiveTexture(GL_TEXTURE0);
      balls->bind();

      glUniform1i(glGetUniformLocation(cool->get_native(), "u_texture"), 0);
      glUniformMatrix4fv(glGetUniformLocation(cool->get_native(), "u_mvp"), 1,
                         GL_FALSE, mvp.cols[0]);

      glBindBuffer(GL_ARRAY_BUFFER, vbo);
      glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * vertex_count,
                      vertices);

      glBindVertexArray(vao);
      glDrawArrays(GL_TRIANGLES, 0, vertex_count);

      vertex_count = 0;
    }

    void r_push_vertex(float x, float y, float u,
                       float v) {
      if (vertex_count == vertex_capacity) {
        r_flush();
      }

      vertices[vertex_count++] = (Vertex){
        .position = {x, y},
        .texcoord = {u, v},
      };
    }

    void r_texture(crystal::Texture *texture) {
      if (balls != texture) {
        r_flush();
        balls = texture;
      }
    }

    void r_mvp(Matrix mat) {
      if (memcmp(&mvp.cols, &mat.cols, sizeof(Matrix)) != 0) {
        r_flush();
        mvp = mat;
      }
    }

    typedef struct {
        // position
        float px, py;
        // texcoords
        float tx, ty, tw, th;
    } Alien;

    void draw_alien(crystal::Texture *tex, Alien a) {
        r_texture(tex);

        float x1 = a.px;
        float y1 = a.py;
        float x2 = a.px + 64;
        float y2 = a.py + 64;

        float u1 = a.tx / tex->get_width();
        float v1 = a.ty / tex->get_height();
        float u2 = (a.tx + a.tw) / tex->get_width();
        float v2 = (a.ty + a.th) / tex->get_height();

        r_push_vertex(x1, y1, u1, v1);
        r_push_vertex(x2, y2, u2, v2);
        r_push_vertex(x1, y2, u1, v2);

        r_push_vertex(x1, y1, u1, v1);
        r_push_vertex(x2, y1, u2, v1);
        r_push_vertex(x2, y2, u2, v2);
    }

    Matrix mat_ortho(float left, float right, float bottom, float top, float znear,
                 float zfar) {
        Matrix m = {0};

        float rl = 1.0f / (right - left);
        float tb = 1.0f / (top - bottom);
        float fn = -1.0f / (zfar - znear);

        m.cols[0][0] = 2.0f * rl;
        m.cols[1][1] = 2.0f * tb;
        m.cols[2][2] = 2.0f * fn;
        m.cols[3][0] = -(right + left) * rl;
        m.cols[3][1] = -(top + bottom) * tb;
        m.cols[3][2] = (zfar + znear) * fn;
        m.cols[3][3] = 1.0f;

        return m;
    }

    TitleScreen::TitleScreen() {}

    TitleScreen::~TitleScreen() {}

    void TitleScreen::on_switch_dispose(void) {
        delete cool;
        free(vertices);

        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
    }

    void TitleScreen::init(void) {
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertex_capacity, NULL,
                       GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                            (void *)offsetof(Vertex, position));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                            (void *)offsetof(Vertex, texcoord));

        cool = new crystal::Shader(fragment, vertex);
        balls = crystal::AssetServer::get_texture("MISSING_TEXTURE");
        vertices = (Vertex*)malloc(sizeof(Vertex) * vertex_capacity);
        mvp = {0};

        batched_sprites = new crystal::BatchedSprites(crystal::BatchingMode::BAKED, 65534);
        batched_sprites->position = glm::dvec2(16.0, 16.0);

        for (int i = 0; i < 128; i++) {
            double x = i * 32.0;
            double y = 0.0;

            if (x >= 1280.0 - 32.0) {
                y = floor(x / (1280.0 - 32.0)) * 32.0;
                x = fmod(x, 1280.0 - 32.0);
            }

            batched_sprites->add_batched_sprite({
                glm::vec4(0.0f, 0.0f, -1.0f, -1.0f),
                glm::dvec2(x, y), glm::dvec2(0.5, 0.5), glm::dvec2(0.0),
                0.0
            });
        }

        batched_sprites->refresh(false);
    }

    void TitleScreen::step(const double delta) {
        // testing scene switch memory leaks, still an issue BUT it's less than 0.1 mib so it's hopefully ok :sob:
        if (crystal::InputServer::is_key_pressed(GLFW_KEY_SPACE) && crystal::Engine::can_transition()) {
            crystal::Engine::switch_scene_to(new TitleScreen());
            return;
        }

        batched_sprites->step(delta);
    }

    void TitleScreen::draw(void) {
        // r_mvp(mat_ortho(0, 1280.0f, 720.0f, 0, -1.0f, 1.0f));

        // Alien ch = {
        //     .px = 0,
        //     .py = 0,
        //     .tx = 0,
        //     .ty = 0,
        //     .tw = 64,
        //     .th = 64,
        // };

        // for (int i = 0; i < 1000; i++) {
        //     ch.px = (sin(i) * 640) + 640;
        //     ch.px -= 32;
        //     ch.py = (cos(i) * 360) + 360;
        //     ch.py -= 32;
        //     draw_alien(balls, ch);
        // }

        // r_flush();

        batched_sprites->draw();
    }
}