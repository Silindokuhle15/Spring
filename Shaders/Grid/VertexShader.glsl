#version 450 core
uniform mat4 MV;
uniform mat4 MVP;

uniform vec3 LightPosition;

layout(location = 0) in vec3 Position;

void main()
 {
    gl_Position = MVP * vec4(Position, 1.0);
}