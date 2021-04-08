#pragma once

#include "../utils/math.h"
#include "Texture.h"

struct Material
{

	vec3 diffuseColor;
	vec3 specularColor;
	float specularExponent;

	SharedTexture texture;

};

