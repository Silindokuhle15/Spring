#version 450 core

uniform vec4 AmbientColor;

in VS_OUT
{
    vec3 color;

} fs_in;

layout(location = 0) out vec4 Color;

void main()
{
    Color = vec4(fs_in.color, 1.0); 
}