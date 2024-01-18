#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <glad/glad.h>

#include "glm/gtc/type_ptr.hpp"

#include "core/file_system.h"
#include "core/rendering/rendering_server.h"
#include "core/rendering/shader.h"

namespace crystal {
    Shader::Shader(const char *fragment_source, const char *vertex_source) {
        _native_shader = RenderingServer::create_native_shader(fragment_source, vertex_source);
    }

    Shader::~Shader() {
        RenderingServer::dispose_native_shader(_native_shader);
    }

    void Shader::use(void) {
        RenderingServer::use_native_shader(_native_shader);
    }

    int Shader::get_uniform_int(const char *key) {
        int value;
        glGetUniformiv(_native_shader, glGetUniformLocation(_native_shader, key), &value);
        return value;
    }

    void Shader::set_uniform_int(const char *key, int value) {
        glUniform1i(glGetUniformLocation(_native_shader, key), value);
    }

    float Shader::get_uniform_float(const char *key) {
        float value;
        glGetUniformfv(_native_shader, glGetUniformLocation(_native_shader, key), &value);
        return value;
    }

    void Shader::set_uniform_float(const char *key, float value) {
        glUniform1f(glGetUniformLocation(_native_shader, key), value);
    }

    void Shader::set_uniform_color(const char *key, Color value) {
        glUniform4f(glGetUniformLocation(_native_shader, key), value.r, value.g, value.b, value.a);
    }

    void Shader::set_uniform_mat4(const char *key, glm::mat4 value) {
        glUniformMatrix4fv(glGetUniformLocation(_native_shader, key), 1, GL_FALSE, glm::value_ptr(value));
    }

    void Shader::set_uniform_vec4(const char *key, glm::vec4 value) {
        glUniform4f(glGetUniformLocation(_native_shader, key), value.x, value.y, value.z, value.w);
    }

    native_shader Shader::get_native(void) {
        return _native_shader;
    }

    Shader *Shader::get_from_file(const char *shader_path) {
        // + 5 for the five characters in .frag and .vert
        char *_fragment_path = (char*) malloc(sizeof(char) * (strlen(shader_path) + 5));
        strcpy(_fragment_path, shader_path);
        strcat(_fragment_path, ".frag");

        char *_vertex_path = (char*) malloc(sizeof(char) * (strlen(shader_path) + 5));
        strcpy(_vertex_path, shader_path);
        strcat(_vertex_path, ".vert");

        Shader *_return_shader = get_from_file(_fragment_path, _vertex_path);

        // I love memory management
        free(_fragment_path);
        free(_vertex_path);

        return _return_shader;
    }

    Shader *Shader::get_from_file(const char *fragment_path, const char *vertex_path) {
        char *_fragment_source = FileSystem::get_file_contents(fragment_path);
        char *_vertex_source = FileSystem::get_file_contents(vertex_path);

        // Debugging shader file loading.
        // printf("%s\n", _fragment_source);
        // printf("%s\n", _vertex_source);

        Shader *_return_shader = new Shader(_fragment_source, _vertex_source);

        free(_fragment_source);
        free(_vertex_source);

        return _return_shader;
    }
}