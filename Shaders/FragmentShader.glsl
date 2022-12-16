#version 450 core

uniform isampler2D tex;
uniform vec4 AmbientColor;

in VS_OUT
{
    vec2 tex_coord;
    vec3 color;

} fs_in;

layout(location = 0) out vec4 Color;

void main()
{
    Color = vec4(fs_in.color, 1.0); 
}