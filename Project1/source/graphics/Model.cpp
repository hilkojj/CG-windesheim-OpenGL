#include "Model.h"

void Model::render(ShaderProgram &shader, PerspectiveCamera &cam)
{
	shader.use();

	auto mvp = cam.combined * transform;
	glUniformMatrix4fv(shader.location("mvp"), 1, GL_FALSE, value_ptr(mvp));

	int i = 0;
	for (auto &mesh : meshes)
	{
		if (!mesh)
			continue;

		mesh->render();
	}

}
