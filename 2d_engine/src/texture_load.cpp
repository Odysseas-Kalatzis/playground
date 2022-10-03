#include <iostream>
#include "texture_load.h"
#include "glad/glad.h"
#define STB_IMAGE_IMPLEMENTATION 
#include "stb_image.h"

Texture_Load::Texture_Load(char const* path, GLuint tex_unit, 
        GLint wrap_s, GLint wrap_t) : texture_unit(tex_unit)
{
    glGenTextures(1, &texture); 
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);  

    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 
            STBI_rgb_alpha);
    if (data)
    {
        // Generate texture on bound opengl texture based on the byte array data.
        // The second argument specifies the mipmap level for which we want to 
        // create a texture for if you want to set each mipmap level manually. 
        // The third argument tells OpenGL the format we want to store the texture.
        // The 7th and 8th argument specify the format of the source image. 
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, 
                GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else 
    {
        std::cout<< "Failed to load texture from given path.\n";
    }
    // Freeing the resources used by stb_image.
    stbi_image_free(data);
}

void Texture_Load::use()
{
    // Set the appropriate texture unit (from 0 to a maximum of 15).
    glActiveTexture(GL_TEXTURE0 + texture_unit);
    glBindTexture(GL_TEXTURE_2D, texture);
}

GLuint Texture_Load::getTextureID()
{
    return texture;
}

