#version 450 core

layout(vertices = ) out;
void main(void)
{
	gl_out[gl_InvocationID].gl_Position =
			gl_in[gl_InvocationID].gl_Position;
}