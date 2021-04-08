#pragma once

#include "ShaderProgram.h"

#include <memory>

struct Texture
{
    const GLuint id;

    Texture(GLuint id);

    void bind(GLuint unit);

    // same as bind() but also sets sampler uniform for shader.
    void bind(GLuint unit, const ShaderProgram& shader, const char* name);

    ~Texture();
};

typedef std::shared_ptr<Texture> SharedTexture;
