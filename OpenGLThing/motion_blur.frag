/*
 * Motion blur fragment shader
 * 
 * Adds 3 frames together to blur
 */
#version 150

in vec2 tCoords;

uniform sampler2D frame1;
uniform sampler2D frame2;
uniform sampler2D frame3;

uniform float w1;
uniform float w2;
uniform float w3;

out vec4 color;

void main(){
    vec4 c1 = w1 * texture(frame1, tCoords);
    vec3 c2 = w2 * texture(frame2, tCoords).xyz;
    vec3 c3 = w3 * texture(frame3, tCoords).xyz;
    color = vec4(c1.xyz + c2 + c3, c1.a);
}