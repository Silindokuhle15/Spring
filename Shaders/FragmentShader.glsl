#version 330 core
in vec3 m_Color;
out vec4 fragColor;

void main(void)
{
	fragColor = vec4(m_Color, 1.0f);
}