#version 450 core

uniform mat4 V;
uniform mat4 VP;

uniform mat4 Model;

uniform vec4 AmbientColor;

uniform vec3 LightPosition;
//uniform vec3 LightColor;

layout(location = 0) in vec3 Position;
layout(location = 2) in vec3 Normal;

out VS_OUT
{
    vec3 color;
} vs_out;

void main()
 {
    float specular_power = 128.0;

    gl_Position = Model *VP * vec4(Position, 1.0);

    vec4 view_p = Model *V * vec4(Position, 1.0);
    vec3 view_n = mat3(Model*V) * Normal;

    vec3 Light_dir = LightPosition - view_p.xyz;
    vec3 V = -view_p.xyz;

    view_n = normalize(view_n);
    Light_dir = normalize(Light_dir);
    V = normalize(V);

    vec3 R = reflect(-Light_dir, view_n);

    vec3 diffuse = max(dot(view_n, Light_dir), 0.0) * vec3(AmbientColor);
    vec3 specular = pow(max(dot(R, V), 0.0), specular_power) * vec3(AmbientColor);

    vs_out.color = vec3(AmbientColor) + diffuse + specular;
}