#pragma once
#include "VertexAttributes.h"

#include <memory>
#include "../Material.h"


struct VertexData
{
    VertexAttributes attributes;
    std::vector<unsigned char> vertices;

    // get a value out of the vertices array
    template <class type>
    inline type &get(int vertI, int attrOffset)
    {
        return *((type *) &(vertices.at(vertI * attributes.getVertSize() + attrOffset)));
    }

    template <class vecType>
    void normalizeVecAttribute(int attrOffset)
    {
        for (int vertI = 0; vertI < vertices.size() / attributes.getVertSize(); vertI++)
        {
            auto &val = get<vecType>(vertI, attrOffset);
            val = normalize(val);
        }
    }

    void removeVertices(int count);

    void addVertices(int count);

    int nrOfVertices() const;
};


class VertexBuffer;
class Mesh;

typedef std::shared_ptr<Mesh> SharedMesh;


struct Mesh : public VertexData
{
    std::string name;

    VertexBuffer *vertBuffer = nullptr;

    std::vector<unsigned short> indices;
    GLenum mode = GL_TRIANGLES;
    int nrOfIndicesToRender = -1; // -1 => all

    Material material;

    void render();

    // removes the vertices + indices that are stored in RAM,
    // but the mesh can still be drawn if it is uploaded to OpenGL/VRAM using a VertBuffer
    // NOTE: vertices & indices are resized to 0!
    void disposeOfflineData();

    ~Mesh();

  private:
      // used for glDrawElementsBaseVertex:
      friend VertexBuffer;
      unsigned int
          nrOfVertsUploadedToBuffer,
          baseVertexInBuffer,
          indicesBufferOffset,
          nrOfIndicesUploadedToBuffer;
};
