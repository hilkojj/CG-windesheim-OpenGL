#include "Scene.h"
#include "utils/files.h"

Scene::Scene()
	: 
	camera(.1, 1000, 1, 1, 90),
	defaultShader("default shader",
		files::readString("assets/shaders/default.vert").c_str(),
		files::readString("assets/shaders/default.frag").c_str()
	)
{
}

void Scene::render()
{
	glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto& model : models)
		model.render(*this);
}
