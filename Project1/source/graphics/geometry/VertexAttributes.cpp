#include "VertexAttributes.h"
#include <stdexcept>
#include <iostream>


unsigned int VertexAttributes::add(const VertAttr& attr)
{
    unsigned int offset = vertSize;
    vertSize += attr.byteSize;
    nrOfComponents += attr.size;

    if (attr.size > 4) throw std::runtime_error("Tried to add a VertAttr with a size > 4. This is not allowed by OpenGL");

    attributes.push_back(attr);

    return offset;
}

VertexAttributes &VertexAttributes::add_(const VertAttr& attr)
{
    add(attr);
    return *this;
}

const VertAttr& VertexAttributes::get(unsigned int i) const
{
    return attributes[i];
}

unsigned int VertexAttributes::nrOfAttributes() const
{
    return attributes.size();
}

unsigned int VertexAttributes::getVertSize() const
{
    return vertSize;
}

unsigned int VertexAttributes::getOffset(const VertAttr& attr) const
{
    unsigned int offset = 0;
    for (const VertAttr& a : attributes)
    {
        if (a.name == attr.name)
            return offset;
        offset += a.byteSize;
    }
    throw std::runtime_error(attr.name + " is not in VertAttributes, add it first with .add(attribute)");
}

bool VertexAttributes::contains(const VertAttr& attr) const
{
    for (const VertAttr& a : attributes)
        if (a.normalized == attr.normalized && a.size == attr.size && a.type == attr.type && a.name == attr.name)
            return true;
    return false;
}

std::ostream& operator<<(std::ostream& stream, const VertexAttributes& attrs)
{
    stream << "[ ";
    for (const VertAttr& a : attrs.attributes)
        stream << "{" << a.name << ", " << a.size << ", " << (a.normalized ? "GL_TRUE" : "GL_FALSE") << "} ";
    return stream << "]";
}

unsigned int VertexAttributes::getNrOfComponents() const
{
    return nrOfComponents;
}
