#include "Mesh.h"
#include "VertexBuffer.h"
#include <stdexcept>
#include <iostream>


void VertexData::removeVertices(int count)
{
    vertices.resize(vertices.size() - count * attributes.getVertSize());
}

void VertexData::addVertices(int count)
{
    vertices.resize(vertices.size() + count * attributes.getVertSize(), 0);
}

int VertexData::nrOfVertices() const
{
    return vertices.size() / attributes.getVertSize();
}



void Mesh::disposeOfflineData()
{
    vertices.resize(0);
    vertices.shrink_to_fit();

    indices.resize(0);
    indices.shrink_to_fit();
}

void Mesh::render()
{
    if (!vertBuffer || !vertBuffer->isUploaded())
        throw std::runtime_error(name + " is not uploaded. Upload it first with a VertBuffer");
    vertBuffer->bind();
 
    glDrawElementsBaseVertex(
        mode,
        nrOfIndicesToRender < 0 ? nrOfIndicesUploadedToBuffer : nrOfIndicesToRender,
        GL_UNSIGNED_SHORT,
        (void*)(uintptr_t) indicesBufferOffset,
        baseVertexInBuffer
    );
}

Mesh::~Mesh()
{
#ifndef PUT_A_SOCK_IN_IT
    std::cout << "Mesh destroyed: " << name << std::endl;
#endif

    if (vertBuffer)
        vertBuffer->onMeshDestroyed();
}

