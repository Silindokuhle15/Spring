#version 330 core
uniform mat4 PerspectiveMatrix;
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vColor;
out vec3 m_Color;

void main(void)
{
	gl_Position = vec4(vPosition,1.0f);
	m_Color = vColor;
}