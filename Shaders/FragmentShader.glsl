#version 450 core

uniform isampler2D tex;
in vec3 color;
in vec2 vs_tex_out;

layout(location = 0) out vec4 Color;

void main()
{
    vec4 fs_tex_out = texture(tex, vs_tex_out);
    Color = fs_tex_out;
    Color = vec4(0.50f, 0.50f, .50f, .50f); 
}