#version 450 core

layout( vertices = 3) out; // set the Tesselation level

void main(void)
{
	if(glInvocationID == 0)
	{
		glTessLevelInner[0] = 5.0;
		glTessLevelOuter[0] = 5.0;
		glTessLevelOuter[1] = 5.0;
		glTessLevelOuter[1] = 5.0;
	}
	gl_out[gl_InvocationID].gl_Position = 
			gl_in[gl_InvocationID].gl_Position;
}