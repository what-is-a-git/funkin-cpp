#version 330 core

out vec4 COLOR;
in vec2 UV;

uniform bool RIGHT;
uniform float VALUE;
uniform vec4 TINT;

void main(void) {
    if ((RIGHT && UV.x <= VALUE) || (!RIGHT && UV.x >= 1.0 - VALUE)) {
        COLOR = TINT;
    } else {
        COLOR = vec4(0.0);
    }
}