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

    for (auto& part : parts)
    {
        part.indices.resize(0);
        part.indices.shrink_to_fit();
    }
}

void Mesh::render(int partI)
{
    if (!vertBuffer || !vertBuffer->isUploaded())
        throw std::runtime_error(name + " is not uploaded. Upload it first with a VertBuffer");
    vertBuffer->bind();
    if (parts.size() <= partI)
        throw std::runtime_error(name + " only has " + std::to_string(parts.size()) + " part(s). Requested: " + std::to_string(partI));
    auto &part = parts.at(partI);

    glDrawElementsBaseVertex(
        part.mode,
        part.nrOfIndicesToRender < 0 ? part.nrOfIndicesUploadedToBuffer : part.nrOfIndicesToRender,
        GL_UNSIGNED_SHORT,
        (void*)(uintptr_t) part.indicesBufferOffset,
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

