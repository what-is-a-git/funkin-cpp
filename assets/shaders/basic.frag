#version 330 core

out vec4 COLOR;
uniform vec4 TINT;

void main(void) {
    COLOR = TINT;
}