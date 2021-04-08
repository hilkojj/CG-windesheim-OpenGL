#pragma once

#include "Mesh.h"

namespace mesh_generators
{

	SharedMesh createQuad(const mat4 &transform = mat4(1), float uvScaling = 1.);

};

