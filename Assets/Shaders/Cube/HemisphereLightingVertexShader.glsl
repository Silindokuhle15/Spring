#version 450 core

uniform mat4 MV;
uniform mat4 MVP;

uniform vec4 AmbientColor;

uniform vec3 LightPosition;
//uniform vec3 LightColor;

layout(location = 0) in vec3 Position;
layout(location = 1) in vec2 TexCoord;
layout(location = 2) in vec3 Normal;

out VS_OUT
{
    vec2 tex_coord;
    vec3 color;
} vs_out;

void main(void)
{
    vec3 pos = vec3(MV * Position);
    vec3 tnorm = normalize()
}