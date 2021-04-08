#include "Texture.h"

Texture::Texture(GLuint id)
    : id(id)
{}

void Texture::bind(GLuint unit)
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::bind(GLuint unit, const ShaderProgram& shader, const char* name)
{
    bind(unit);
    glUniform1i(shader.location(name), unit);
}

Texture::~Texture()
{
    glDeleteTextures(1, &id);
}
