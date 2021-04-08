#pragma once

#include "Mesh.h"

/**
 * A class that encapsulates OpenGL VertexArrayObjects, VertexBufferObjects, IndexBufferObjects and Instanced Arrays
 */
class VertexBuffer
{
public:

    GLenum vboUsage = GL_STATIC_DRAW;
    GLenum iboUsage = GL_STATIC_DRAW;

    // creates a VertBuffer for meshes with these attributes
    static VertexBuffer *with(VertexAttributes& attributes);

    // try not to use this. It is more efficient to put more meshes (with the same VertAttributes) in 1 VertBuffer
    static void uploadSingleMesh(SharedMesh mesh);

    // adds mesh to Meshes that are going to be uploaded when upload() is called.
    VertexBuffer &add(SharedMesh mesh);

    /**
     * upload all added Meshes to OpenGL, after uploading the Meshes can be drawn.
     *
     * if disposeOfflineData is true then mesh.disposeOfflineData() will be called for each uploaded mesh.
     **/
    void upload(bool disposeOfflineData);

    bool isUploaded() const;

    void bind();

    ~VertexBuffer();

private:

    VertexBuffer(VertexAttributes &attributes);

    // returns wether the stored vertex data is actually used by Meshes
    bool inUse() const;

    void setAttrPointersAndEnable(VertexAttributes &attributes, unsigned int divisor = 0, unsigned int locationOffset = 0);

    // ids of the VertexArrayObject, VertexBufferObject and IndexBufferObject
    GLuint vaoId = 0, vboId = 0, iboId;

    GLuint nrOfVerts = 0, nrOfIndices = 0;

    std::vector<std::weak_ptr<Mesh>> meshes;

    VertexAttributes attrs;

    bool uploaded = false;

    friend Mesh::~Mesh();
    void onMeshDestroyed(); // Called by ~Mesh()

};


