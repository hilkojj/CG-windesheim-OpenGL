#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string>

class ShaderProgram
{
public:
    ShaderProgram(std::string name, const char* vertSource, const char* fragSource);

    ~ShaderProgram();

    bool compiled() const;
    GLuint id() const;
    GLuint location(const char* uniformName) const;

    void use();

protected:
    GLuint programId;
    std::string name;
    bool compiled_;

    void compile(const char* vertSource, const char* fragSource);

    void compileAndAttach(const char* source, GLuint shaderId, const char* shaderType);
};
