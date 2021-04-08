#pragma once

#include "Mesh.h"

namespace mesh_generators
{

	SharedMesh createQuad(const mat4 &transform = mat4(1), const vec2 &uvScaling = vec2(1));

	SharedMesh createSphere(int segments = 32, int rings = 16, const mat4& transform = mat4(1), const vec2& uvScaling = vec2(1));

	SharedMesh createCylinder(int segments = 32, const mat4& transform = mat4(1), const vec2& uvScaling = vec2(1));

};

