#include "ShaderProgram.h"

#include <vector>
#include <iostream>

ShaderProgram::ShaderProgram(std::string name, const char* vertSource, const char* fragSource)

    : name(std::move(name))
{
    compile(vertSource, fragSource);
}

void ShaderProgram::compile(const char* vertSource, const char* fragSource)
{
    programId = glCreateProgram();
    GLuint vertShaderId = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragShaderId = glCreateShader(GL_FRAGMENT_SHADER);

    compileAndAttach(vertSource, vertShaderId, "VERTEX");
    compileAndAttach(fragSource, fragShaderId, "FRAGMENT");

    glLinkProgram(programId);

    // check program:
    GLint success = GL_FALSE;
    int logLength;
    glGetProgramiv(programId, GL_COMPILE_STATUS, &success);
    glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 1)
    {
        std::vector<char> log(logLength + 1);
        glGetProgramInfoLog(programId, logLength, NULL, &log[0]);
        std::cout << name << " compilation log:\n"
            << &log[0] << std::endl;
    }

    glDetachShader(programId, vertShaderId);
    glDetachShader(programId, fragShaderId);

    glDeleteShader(vertShaderId);
    glDeleteShader(fragShaderId);

    compiled_ = success;
}

void ShaderProgram::compileAndAttach(const char* source, GLuint shaderId, const char* shaderType)
{
    const char* sources[] = { source };

    glShaderSource(shaderId, 1, sources, NULL);
    glCompileShader(shaderId);
    glAttachShader(programId, shaderId);
}

bool ShaderProgram::compiled() const
{
    return compiled_;
}

GLuint ShaderProgram::id() const
{
    return programId;
}

GLuint ShaderProgram::location(const char* uniformName) const
{
    return glGetUniformLocation(programId, uniformName);
}

void ShaderProgram::use()
{
    glUseProgram(programId);
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(programId);
}