#pragma once

#include <list>
#include "utils/math.h"
#include "graphics/Model.h"
#include "graphics/ShaderProgram.h"
#include "graphics/camera/PerspectiveCamera.h"

struct Scene
{
	std::list<Model> models;
	vec3 sunDirection = mu::X;
	vec3 backgroundColor = vec3(.1, .1, .6);

	PerspectiveCamera camera;

	ShaderProgram defaultShader;

	Scene();

	void render();
};
