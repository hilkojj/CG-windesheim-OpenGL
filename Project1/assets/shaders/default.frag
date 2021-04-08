#version 430 core

in vec3 vColor;

uniform vec3 diffuse;
uniform vec3 specular;
uniform float specularExponent;

void main()
{
    gl_FragColor = vec4(diffuse, 1.0);
}
