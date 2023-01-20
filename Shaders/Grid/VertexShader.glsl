#version 450 core
uniform mat4 V;
uniform mat4 VP;
uniform mat4 Model;

uniform vec3 LightPosition;

layout(location = 0) in vec3 Position;

void main()
 {
    gl_Position = Model * VP * vec4(Position, 1.0);
}