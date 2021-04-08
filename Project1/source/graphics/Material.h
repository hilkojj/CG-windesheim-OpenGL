#pragma once

#include "../utils/math.h"
#include "Texture.h"

struct Material
{

	vec3 diffuseColor = vec3(1);
	vec3 specularColor = vec3(0);
	float specularExponent = 1;

	SharedTexture texture;

};

