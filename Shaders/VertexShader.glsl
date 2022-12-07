#version 450 core
uniform mat4 MVP;

layout(location = 0) in vec3 Position;
layout(location = 1) in vec2 TexCoord;

out vec3 color;
out vec2 vs_tex_out;

void main()
 {
    gl_Position = MVP * vec4(Position, 1.0);
    vs_tex_out = TexCoord;
}