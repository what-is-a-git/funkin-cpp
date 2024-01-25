#version 330 core

layout (location = 0) in vec4 DATA;

uniform mat4 PROJECTION;
uniform mat4 TRANSFORM;

uniform vec4 SOURCE_RECT;

out vec2 UV;

void main(void) {
    gl_Position = PROJECTION * TRANSFORM * vec4(DATA.xy, 0.0, 1.0);
    UV = mix(SOURCE_RECT.xy, SOURCE_RECT.zw, DATA.zw);
}