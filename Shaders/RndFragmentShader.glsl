#version 450 core

uniform sampler2D u_Tex;
layout(location = 0) out vec4 color;

in VS_OUT
{
	vec2 TexCoord;
}ps_in;

void main()
{
	//color = vec4(ps_in.TexCoord, 0.0f, 1.0f);
	color = texture(u_Tex, ps_in.TexCoord );
}