#version 450 core
layout(binding = 0) uniform sampler2D u_Tex;
layout(binding = 1) uniform image2D u_Tex1;

uniform int EnableLighting;
uniform vec3 LightColor;
uniform vec3 SkyColor;
uniform vec3 GroundColor;
uniform float factor;

uniform int EnableTexture;

uniform vec3 AmbientColor;
uniform vec3 DiffuseColor;
uniform vec3 SpecularColor;

uniform mat4 NormalMatrix;

layout(location = 0) out vec4 color;
layout(location = 1) out uint another_col;
in VS_OUT
{
	vec2 TexCoord;
	uint ID;
	vec3 Normal;
	vec3 LightDirection;

}ps_in;

void main()
{

	vec4 object_id = normalize(vec4(ps_in.ID, 0.0f, 0.0f, 0.0f, 1.0f));
	ivec2 p = ivec2(gl_FragCoord.xy);
	imageStore(u_Tex1, p, object_id);

	vec4 tnorm = NormalMatrix * vec4(ps_in.Normal, 1.0f);
	vec3 tnorm_3 = vec3( tnorm.x,tnorm.y, tnorm.z);

	float costheta = dot(tnorm_3, ps_in.LightDirection);
	float a = costheta * 0.5 + 0.5;
	vec3 color_3 = mix(GroundColor, SkyColor, a);
	
	float diffuse = max(0.0, dot(ps_in.Normal, ps_in.LightDirection));
	vec3 u_amb = vec3(AmbientColor[0],AmbientColor[1],AmbientColor[2]);

	if(EnableTexture == 1 )
	{
			vec4 u_color = texture(u_Tex, ps_in.TexCoord );
			u_amb[0] = u_color[0];
			u_amb[1] = u_color[1];
			u_amb[2] = u_color[2];
	}

	color = vec4(u_amb, 1.0f);
	vec3 scattered_light = u_amb + LightColor * diffuse;
	vec3 final_color = mix(scattered_light, color_3, factor);

	//color = vec4(final_color, 1.0f);
	
	//color = vec4(ps_in.Normal, 1.0f);
	color = vec4(ps_in.TexCoord, 1/ps_in.ID, 1.0f);
	
}