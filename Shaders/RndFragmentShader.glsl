#version 450 core
uniform sampler2D u_Tex;

uniform int EnableLighting;
uniform int EnableTexture;

uniform vec3 AmbientColor;
uniform vec3 DiffuseColor;
uniform vec3 SpecularColor;

layout(location = 0) out vec4 color;

in VS_OUT
{
	vec2 TexCoord;
}ps_in;

void main()
{
	color = vec4(AmbientColor, 1.0f);
	if(EnableTexture == 1 )
	{
			color = texture(u_Tex, ps_in.TexCoord );
	}
	else
	{
			color = vec4(AmbientColor + DiffuseColor, 1.0f);
	}
}