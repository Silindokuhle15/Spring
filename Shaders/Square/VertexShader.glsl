#version 450 core
uniform mat4 MVP;

layout(location = 0) in  vec2 Position;
layout(location = 1) in  vec2 TexCoord;

out VS_OUT
{
	vec2 TexCoord;
}vs_out;

void main()
{
	gl_Position = MVP * vec4(Position,0.0, 1.0f);
	vs_out.TexCoord = TexCoord;
}