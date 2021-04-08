#pragma once
#include "geometry/Mesh.h"
#include "../utils/math.h"

class Scene;

struct Model
{

	std::string name;

	std::vector<SharedMesh> meshes;

	bool shadeless = false;

	mat4 transform = mat4(1);

	void render(Scene &);

};

