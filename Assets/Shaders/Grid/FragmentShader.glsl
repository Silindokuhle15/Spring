#version 450 core

uniform vec3 LightPosition;

layout(location = 0) out vec4 Color;

void main()
 {
	Color = vec4(1.0,0.0, 0.0, 1.0);
}