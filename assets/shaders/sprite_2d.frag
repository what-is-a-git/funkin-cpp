#version 330 core

out vec4 COLOR;
in vec2 UV;
uniform sampler2D TEXTURE;
uniform vec4 TINT;

void main(void) {
    // Slight optimization because we don't run texture(TEXTURE, UV)?
    if (TINT.a <= 0.0) {
        COLOR = vec4(0.0);
        return;
    }

    COLOR = texture(TEXTURE, UV) * TINT;
}