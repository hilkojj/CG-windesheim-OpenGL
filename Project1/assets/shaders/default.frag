#version 430 core

in vec3 vPosition;
in vec3 vNormal;

uniform vec3 camPosition;
uniform vec3 sunDirection;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float specularExponent;

// PHONG
void main()
{
    float diffuseLight = (dot(sunDirection, vNormal) + 1.) * .5;    // 0 - 1

    diffuseLight = max(.1, diffuseLight);   // add ambient light

    gl_FragColor = vec4(diffuse * diffuseLight, 1.0);


    // specularity:

    vec3 viewDir = normalize(camPosition - vPosition);
    vec3 reflectDir = reflect(-sunDirection, normalize(vNormal));   // vNormal needs to be normalized because the interpolation done by OpenGL changes the length of the vector

    float specularity = pow(max(dot(viewDir, reflectDir), 0.), specularExponent);

    gl_FragColor.rgb += specular * specularity;
}
