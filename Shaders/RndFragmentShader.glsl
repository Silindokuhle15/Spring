#version 450 core

uniform sampler2D tex;
layout(location = 0) out vec4 color;

in VS_OUT
{
	vec2 TexCoord;

}ps_in;

void main()
{
	color  = vec4(ps_in.TexCoord, 0.0f, 1.0f);
	//color  = vec4(.7f, .7f,.7f, 1.0f);
}