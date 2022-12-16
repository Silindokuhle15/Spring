#version 450 core

uniform mat4 MVP;

layout (triangles, equal_spacing, cw) in;

void main(void)
{
	float u = gl_TessCoord.x;
	float v = gl_TessCoord.y;

	gl_Position =	MVP * (u * gl_in[0].gl_Position + 
						  (v * gl_in[1].gl_Position );
						  //(gl_TessCoord.z * gl_in[2].gl_Position );
}