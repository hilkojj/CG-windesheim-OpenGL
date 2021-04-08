#include "model_loader.h"

#include "../external/OBJ_Loader.h"
#include "../graphics/geometry/VertexBuffer.h"

void model_loader::loadIntoModel(Model &model, const char *objPath)
{
	objl::Loader loader;
	loader.LoadFile(objPath);

	VertexAttributes attributes;
	int posOff = attributes.add({ "position", 3 });
	int norOff = attributes.add({ "normal", 3 });
	int texOff = attributes.add({ "texture_coords", 3 });

	auto buffer = VertexBuffer::with(attributes);

	for (auto &loadedMesh : loader.LoadedMeshes)
	{
		SharedMesh mesh(new Mesh);

		mesh->name = loadedMesh.MeshName;
		mesh->attributes = attributes;

		for (auto i : loadedMesh.Indices)
			mesh->indices.push_back(i);

		mesh->addVertices(loadedMesh.Vertices.size());

		int vertI = 0;
		for (auto& vert : loadedMesh.Vertices)
		{
			mesh->get<vec3>(vertI, posOff) = vec3(vert.Position.X, vert.Position.Y, vert.Position.Z);
			mesh->get<vec3>(vertI, norOff) = vec3(vert.Normal.X, vert.Normal.Y, vert.Normal.Z);
			
			mesh->get<vec2>(vertI, texOff) = vec2(vert.TextureCoordinate.X, vert.TextureCoordinate.Y);

			vertI++;
		}
		buffer->add(mesh);
		model.meshes.push_back(mesh);
	}

	buffer->upload(true);
}
