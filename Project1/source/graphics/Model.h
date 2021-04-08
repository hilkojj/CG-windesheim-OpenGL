#pragma once
#include "geometry/Mesh.h"
#include "../utils/math.h"
#include "ShaderProgram.h"
#include "camera/PerspectiveCamera.h"

struct Model
{

	std::string name;

	std::vector<SharedMesh> meshes;

	mat4 transform = mat4(1);

	void render(ShaderProgram &, PerspectiveCamera &);

};

