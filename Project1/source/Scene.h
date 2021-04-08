#pragma once

#include <list>
#include "utils/math.h"
#include "graphics/Model.h"
#include "graphics/ShaderProgram.h"
#include "graphics/camera/PerspectiveCamera.h"

struct Scene
{
	std::list<Model> models;
	vec3 sunDirection = normalize(vec3(1, .7, 0));
	vec3 backgroundColor = vec3(.2, .4, .8);

	PerspectiveCamera camera;

	ShaderProgram
		defaultShader,
		shadelessShader;

	Scene();

	void render();
};
