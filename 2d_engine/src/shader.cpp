#include <iostream>
#include <cstdlib>
#include <glad/glad.h>
#include "include/shader.h"

Shader::Shader(char const* vertexPath, char const* fragmentPath)
{
    char* vShaderCode = (char*) malloc(sizeof(*vShaderCode)*4096);
    char* fShaderCode = (char*) malloc(sizeof(*fShaderCode)*4096);

    char ch; // Stores character when reading file.
    int i = 0; // Counts the numbers of characters of read files.

    // Reading the vertex shader.
    FILE* vFile = fopen(vertexPath, "r");
    if (vFile == NULL)
    {
        fputs("Failed to open vertex shader\n", stderr);
    }

    int vSize = 4096;
    while ((ch = fgetc(vFile)) != EOF)
    {
        if (i >= (vSize-1))
        {
            vSize *= 2;
            vShaderCode = (char*) 
                reallocarray(vShaderCode, vSize, sizeof(*vShaderCode));
        }
        vShaderCode[i++] = ch; 
    }
    vShaderCode[i] = '\0';

    fclose(vFile);

    // Reading the fragment shader.
    FILE* fFile = fopen(fragmentPath, "r");
    if (fFile == NULL)
    {
        fputs("Failed to open fragment shader\n", stderr);
    }

    i = 0;

    int fSize = 4096;
    while ((ch = fgetc(fFile)) != EOF)
    {
        if (i >= (fSize-1))
        {
            fSize *= 2;
            fShaderCode = (char*) 
                reallocarray(fShaderCode, fSize, sizeof(*fShaderCode));
        }
        fShaderCode[i++] = ch; 
    }
    fShaderCode[i] = '\0';

    fclose(fFile);

    // CREATE OUR OPENGL SHADERS.
    char const* vertexShaderSource = vShaderCode;
    char const* fragmentShaderSource = fShaderCode;

    // Vertex Shader:
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // 1 stands for 1 source file used for the vertex shader.
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Check for shader compile errors.
    int success;
    char infoLog[512];
    
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" 
            << infoLog << '\n';  
    }

    // Fragment Shader:
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // 1 stands for 1 source file used for the fragment shader.
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Check for shader compile errors.
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" 
            << infoLog << '\n';
    }

    // Link Shaders:
    id = glCreateProgram();
    glAttachShader(id, vertexShader);
    glAttachShader(id, fragmentShader);
    glLinkProgram(id);

    // Check for linking errors:
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(id, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
            << infoLog << '\n';
    }

    // Clean-up.
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    free(vShaderCode);
    free(fShaderCode);
}

GLuint Shader::getShaderID()
{
    return id;
}

void Shader::use()
{
    glUseProgram(id);
}

void Shader::destroy()
{
    glDeleteProgram(id);
}

void Shader::setBool(char const* name, bool value) const
{
    glUniform1i(glGetUniformLocation(id, name), (int)value);
}

void Shader::setInt(char const* name, int value) const
{
    glUniform1i(glGetUniformLocation(id, name), value);
}


void Shader::setFloat(char const* name, float value) const
{
    glUniform1f(glGetUniformLocation(id, name), value);
}

void Shader::setVec4f(char const* name, float value[4]) const
{
    glUniform4f(glGetUniformLocation(id, name), value[0], value[1],
            value[2], value[3]);
}

void Shader::setMat4f(char const* name, float value[16]) const
{
    // Function parameters: location, count, transpose, value.
    glUniformMatrix4fv(glGetUniformLocation(id, name), 1, GL_FALSE, value);
}

