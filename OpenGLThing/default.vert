#version 150 core

in vec3 vert_position;
in vec3 in_color;
in vec2 texCoords;

uniform mat4 mWorld;
uniform mat4 mView;
uniform mat4 mProj;

out vec3 color;

void main() {
    vec2 t = texCoords;
    gl_Position = mProj * mView * mWorld * vec4(vert_position, 1);
    color = in_color;
}