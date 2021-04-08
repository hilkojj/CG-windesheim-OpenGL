#pragma once

#include "Mesh.h"

namespace mesh_generators
{

	SharedMesh createQuad(const mat4 &transform = mat4(1), float uvScaling = 1.);

	SharedMesh createSphere(int segments = 32, int rings = 16, const mat4& transform = mat4(1), float uvScaling = 1.);

};

