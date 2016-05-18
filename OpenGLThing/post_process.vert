/*
 * Post-processing vertex shader
 * in_vertex = out_vertex
 */

#version 150

in vec3 vert_position;
in vec3 in_color;
in vec2 texCoords;

out vec2 tCoords;

void main() {
    vec3 pcolor = 2*in_color;
    gl_Position = vec4(vert_position,1.0);
    tCoords = texCoords;
}