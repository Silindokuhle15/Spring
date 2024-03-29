#version 450 core
uniform mat4 VP;
uniform mat4 V;

uniform mat4 Model;

layout(location = 0) in  vec3 Position;
layout(location = 1) in  vec2 TexCoord;

out VS_OUT
{
	vec2 TexCoord;
}vs_out;

void main()
{
	gl_Position = Model * VP * vec4(Position, 1.0f);
	vs_out.TexCoord = TexCoord;
}