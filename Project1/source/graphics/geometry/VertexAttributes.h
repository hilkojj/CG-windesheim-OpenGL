#pragma once

#include <vector>
#include <string>
#include <GL/glew.h>
#include <GL/freeglut.h>

struct VertAttr
{
    std::string name;
    GLuint size, byteSize = sizeof(float) * size;
    GLenum type = GL_FLOAT;
    GLboolean normalized = GL_FALSE;
};

class VertexAttributes
{

public:

    /**
     * adds the attribute, and returns the offset in bytes
     *
     * Example:
     * 
     * attrs.add({"position", 3}); // returns 0
     * attrs.add({"normal", 3}); // returns 3 * sizeof(float)
     */
    unsigned int add(const VertAttr& attr);

    // same as above but returns this. (For chaining)
    VertexAttributes& add_(const VertAttr& attr);

    const VertAttr& get(unsigned int i) const;

    unsigned int nrOfAttributes() const;

    // returns the number of bytes per vertex. [POSITION, NORMAL] will return 6 * sizeof(float).
    unsigned int getVertSize() const;

    // returns the number of components per vertex. [POSITION, NORMAL] will return 6.
    unsigned int getNrOfComponents() const;

    // returns the offset (in bytes)
    unsigned int getOffset(const VertAttr& attr) const;

    bool contains(const VertAttr& attr) const;

    // to string
    friend std::ostream& operator<<(std::ostream& stream, const VertexAttributes &attrs);

private:
    unsigned int vertSize = 0, nrOfComponents = 0;
    std::vector<VertAttr> attributes;
};

