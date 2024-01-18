#version 330 core

layout (location = 0) in vec4 DATA;

uniform mat4 PROJECTION;
uniform mat4 TRANSFORM;

out vec2 UV;

void main(void) {
    gl_Position = PROJECTION * TRANSFORM * vec4(DATA.xy, 0.0, 1.0);
    UV = DATA.zw;
}