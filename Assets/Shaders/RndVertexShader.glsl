#version 450 core

uniform vec3 LightPosition;
uniform mat4 VP;

uniform mat4 Model;
uniform float delta;

layout(location = 0) in  vec3 Position;
layout(location = 1) in  vec2 TexCoord;
layout(location = 2) in  uint ID;
layout(location = 3) in  vec3 Normal;

out VS_OUT
{
	vec2 TexCoord;
	uint ID;
	vec3 Normal;
	vec3 LightDirection;

}vs_out;

void main()
{
	vec3 light_direction = normalize(LightPosition - Position);
	gl_Position = Model * VP * vec4(Position, 1.0f) * delta;
	vs_out.TexCoord = TexCoord;
	vs_out.ID = ID;
	vs_out.Normal = Normal;
	vs_out.LightDirection = light_direction;
}