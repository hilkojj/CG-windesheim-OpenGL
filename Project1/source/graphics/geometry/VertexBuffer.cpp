#include "VertexBuffer.h"
#include <stdexcept>
#include <iostream>

VertexBuffer *VertexBuffer::with(VertexAttributes& attributes)
{
    return new VertexBuffer(attributes);
}

void VertexBuffer::uploadSingleMesh(SharedMesh mesh)
{
    with(mesh->attributes)->add(mesh).upload(false);
}

VertexBuffer::VertexBuffer(VertexAttributes& attributes)

    : attrs(attributes)
{
    glGenVertexArrays(1, &vaoId);
}

void VertexBuffer::bind()
{
    glBindVertexArray(vaoId);
}

VertexBuffer &VertexBuffer::add(SharedMesh mesh)
{
    if (mesh->vertBuffer)
        throw std::runtime_error(mesh->name + " was already added to a VertexBuffer");

    std::cout << "Adding " << mesh->name << " to VertexBuffer " << vaoId << "\n";
    meshes.push_back(mesh);

    mesh->baseVertexInBuffer = nrOfVerts;
    mesh->nrOfVertsUploadedToBuffer = mesh->nrOfVertices();
    nrOfVerts += mesh->nrOfVertices();

    mesh->indicesBufferOffset = nrOfIndices * sizeof(GLushort);
    mesh->nrOfIndicesUploadedToBuffer = mesh->indices.size();
    nrOfIndices += mesh->nrOfIndicesUploadedToBuffer;

    mesh->vertBuffer = this;
    return *this;
}

void VertexBuffer::upload(bool disposeOfflineData)
{
    if (vboId)
        throw std::runtime_error("VertexBuffer already uploaded");

#ifndef PUT_A_SOCK_IN_IT
    std::cout << "Uploading vbo\n";
#endif
    bind();

    glGenBuffers(1, &vboId);    // create VertexBuffer
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, nrOfVerts * attrs.getVertSize(), NULL, vboUsage);

    glGenBuffers(1, &iboId);    // create IndexBuffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * nrOfIndices, NULL, iboUsage);

    GLuint vertsOffset = 0, indicesOffset = 0;
    for (std::weak_ptr<Mesh> m : meshes)
    {
        if (m.expired())
            throw std::runtime_error("Trying to upload a VertexBuffer whose Meshes are already destroyed");

        SharedMesh mesh = m.lock();

        if (mesh->nrOfVertices() != mesh->nrOfVertsUploadedToBuffer)
            throw std::runtime_error("Mesh vertices have resized between .add() and .upload() for " + mesh->name);

        GLuint vertsSize = mesh->nrOfVertsUploadedToBuffer * attrs.getVertSize();
        glBufferSubData(GL_ARRAY_BUFFER, vertsOffset, vertsSize, mesh->vertices.data());
        vertsOffset += vertsSize;

        if (mesh->indices.size() != mesh->nrOfIndicesUploadedToBuffer)
            throw std::runtime_error("Mesh indices have resized between .add() and .upload() for  " + mesh->name);

        GLuint indicesSize = mesh->nrOfIndicesUploadedToBuffer * sizeof(GLushort);

        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, indicesOffset, indicesSize, mesh->indices.data());
        indicesOffset += indicesSize;

        if (disposeOfflineData) mesh->disposeOfflineData();
    }
    setAttrPointersAndEnable(attrs);
    uploaded = true;
}

void VertexBuffer::setAttrPointersAndEnable(VertexAttributes& attrs, unsigned int divisor, unsigned int locationOffset)
{
    GLint offset = 0;
    for (int i = locationOffset; i < locationOffset + attrs.nrOfAttributes(); i++)
    {
        auto& attr = attrs.get(i - locationOffset);
        glDisableVertexAttribArray(i);
        switch (attr.type)
        {
        case GL_INT:
        case GL_UNSIGNED_INT:
        case GL_SHORT:
        case GL_UNSIGNED_SHORT:
        case GL_BYTE:
        case GL_UNSIGNED_BYTE:

            glVertexAttribIPointer(
                i,                                    // location of attribute that can be used in vertex shaders. eg: 'layout(location = 0) in vec3 position'
                attr.size,                            // size.
                attr.type,                             // type
                attrs.getVertSize(),                  // stride
                (void*)(uintptr_t)offset             // offset
            );
            break;
        default:
            glVertexAttribPointer(
                i,                                    // location of attribute that can be used in vertex shaders. eg: 'layout(location = 0) in vec3 position'
                attr.size,                            // size.
                attr.type,                             // type
                attr.normalized ? GL_TRUE : GL_FALSE, // normalized?
                attrs.getVertSize(),                  // stride
                (void*)(uintptr_t)offset             // offset
            );
        }
        glEnableVertexAttribArray(i);
        offset += attr.byteSize;

        if (divisor)
            glVertexAttribDivisor(i, divisor);
    }
}

void VertexBuffer::onMeshDestroyed()
{
#ifndef PUT_A_SOCK_IN_IT
    std::cout << "A mesh in this VB was destroyed\n";
#endif
    if (!inUse()) delete this;
}

bool VertexBuffer::isUploaded() const
{
    return uploaded;
}

bool VertexBuffer::inUse() const
{
    for (std::weak_ptr<Mesh> m : meshes)
        if (!m.expired()) return true;
    return false;
}

VertexBuffer::~VertexBuffer()
{
    if (inUse())
    {
        std::cerr << "WARNING: Deleting a VertexBuffer that is still in use by [";
        bool first = true;

        for (std::weak_ptr<Mesh> m : meshes)
            if (!m.expired())
            {
                if (!first)
                    std::cerr << ", ";
                std::cerr << m.lock()->name;
                first = false;
            }
        std::cerr << "]" << std::endl;
    }
#ifndef PUT_A_SOCK_IN_IT
    std::cout << "Deleting VertexBuffer: vao & vbo & ibo\n";
#endif
    glDeleteVertexArrays(1, &vaoId);
    glDeleteBuffers(1, &vboId);
    glDeleteBuffers(1, &iboId);
}
