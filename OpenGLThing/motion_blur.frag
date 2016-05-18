/*
 * Motion blur fragment shader
 * 
 * Adds 3 frames together to blur
 */
#version 150

in vec2 tCoords;

const int MAX_FRAMES = 16;

uniform sampler2D frames[MAX_FRAMES];
uniform float weights[MAX_FRAMES];
uniform int num_frames;

out vec4 color;

void main(){
    vec4 sum = vec4(0,0,0,1);
    float w = 0;
    for (int i = 0; i < num_frames; i++) {
        w += weights[i];
        sum.xyz += weights[i]*texture(frames[i],tCoords).xyz;
    }
    color = sum/w;
}