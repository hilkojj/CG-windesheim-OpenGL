#version 430 core

precision mediump float;

in vec2 vTextureCoord;

uniform vec3 diffuse;
uniform int useTexture;
uniform sampler2D diffuseTexture;

// SHADELESS
void main()
{
    vec3 diffuseColor = diffuse;
    if (useTexture == 1)
        diffuseColor = texture(diffuseTexture, vTextureCoord).rgb;

    gl_FragColor = vec4(diffuseColor, 1.0);
}
