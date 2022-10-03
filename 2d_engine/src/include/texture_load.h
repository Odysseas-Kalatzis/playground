#pragma once

#include <glad/glad.h>

class Texture_Load
{
private:
    GLuint texture;
    GLuint texture_unit;

public:
    GLint width; 
    GLint height;
    GLint nrChannels; 
    // Generates a texture. Alternatives for wrap_s and wrap_t include:
    // GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE
    Texture_Load(char const* path, GLuint tex_unit=0,
            GLint wrap_s=GL_REPEAT, GLint wrap_t=GL_REPEAT);
    Texture_Load(Texture_Load const& tex_load) = delete;
    Texture_Load& operator=(Texture_Load const& tex_load) = delete;
    void use(); // Activate the texture.
    GLuint getTextureID();
    GLuint getTextureUnit();
};

