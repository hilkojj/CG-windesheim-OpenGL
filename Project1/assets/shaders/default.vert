#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform mat4 mvp;

out vec3 vNormal;

void main()
{
    gl_Position = mvp * vec4(position, 1.0);

    vNormal = normal;
}
