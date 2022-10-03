#pragma once

#include <glad/glad.h>

class Shader
{
private:
    // The program ID.
    GLuint id;

public:
    // Constructor that reads and builds the shader.
    Shader(char const* vertexPath, char const* fragmentPath);

    GLuint getShaderID();

    // Use/activate the shader.
    void use();

    // Used for clean-up.
    void destroy();

    // Utility uniform functions.
    void setBool(char const* name, bool value) const;
    void setInt(char const* name, int value) const;
    void setFloat(char const* name, float value) const;
    void setVec4f(char const* name, float value[4]) const;
    void setMat4f(char const* name, float value[16]) const;
};

