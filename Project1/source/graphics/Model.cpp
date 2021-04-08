#include "Model.h"
#include "../Scene.h"

void Model::render(Scene &scene)
{
	auto &shader = scene.defaultShader;
	auto& cam = scene.camera;

	shader.use();

	mat4 mvp = cam.combined * transform;
	glUniformMatrix4fv(shader.location("mvp"), 1, GL_FALSE, value_ptr(mvp));
	glUniformMatrix4fv(shader.location("transform"), 1, GL_FALSE, value_ptr(transform));
	glUniform3fv(shader.location("sunDirection"), 1, &scene.sunDirection[0]);
	glUniform3fv(shader.location("camPosition"), 1, &scene.camera.position[0]);

	int i = 0;
	for (auto &mesh : meshes)
	{
		if (!mesh)
			continue;

		glUniform3fv(shader.location("diffuse"), 1, &mesh->material.diffuseColor[0]);
		glUniform3fv(shader.location("specular"), 1, &mesh->material.specularColor[0]);
		glUniform1f(shader.location("specularExponent"), mesh->material.specularExponent);

		mesh->render();
	}

}
