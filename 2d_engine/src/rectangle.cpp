#include <stdio.h>
#include <glad/glad.h>
#include "include/rectangle.h"

// Returns struct of (VBO, EBO, VAO) for rectangle.
vbo_ebo_vao makeRectangle()
{
    // Creating the vertices for our rectangle.
    float vertices[] = {
        //   Positions           Colors       Tex Coords
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,     // Bottom left.
         0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,     // Bottom right.
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,     // Top right.
        -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f      // Top left.
    };

    // The indices to tell OpenGL the order at which we want to
    // draw our triangles. Note that we start from index 0.
    unsigned int indices[] = {
        0, 1, 2,    // First triangle.
        2, 3, 0     // Second triangle.
    };

    unsigned int VBO;
    unsigned int EBO;
    unsigned int VAO;

    // First argument is how many VAOs we would like to generate.
    // Second argument is the VAOs that we store the resulting ID.
    glGenVertexArrays(1, &VAO);
    // Same as above but for VBOs.
    glGenBuffers(1, &VBO);
    // Same as above but for EBOs.
    glGenBuffers(1, &EBO);
    
    // Bind the Vertex Array Object first, then bind and set 
    // vertex buffer(s), and then configure vertex attribute(s).
    glBindVertexArray(VAO);

    // OpenGL has many types of buffer objects and the buffer type 
    // of a vertex buffer object is GL_ARRAY_BUFFER.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // GL_STREAM_DRAW: The data is set only once and used few times.
    // GL_STATIC_DRAW: The data is set only once and used many times.
    // GL_DYNAMIC_DRAW: The data is changed a lot and used many times.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // The buffer object of an EBO is GL_ELEMENT_ARRAY_BUFFER
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Telling OpenGL how to interpret the vertex data per vertex attribute.
    // Params: 
    // Index: Specifies the index of the vertex attribute.
    // Size: Specifies the number of components per vertex attribute.
    // Type: Specifies the data type of each component in the array.
    // Normalised: Specifies whether data should be in -1 to 1 range.
    // Stride: Specifies the byte offset between consecutive vertex attributes.
    // Pointer: Specifies an offset of the first component.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0); // Position attribute.

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
            (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1); // Color attribute.

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
            (void*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2); // Texture coordinate attribute.

    // The call to glVertexAttribPointer registered VBO as the vertex attribute's
    // bound vertex buffer object so afterwards we can safely unbind.
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Remember: Do NOT unbind the EBO while a VAO is active as the bound
    // element buffer object IS stored in the VAO; keep the EBO bound.
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Do not do this.

    // We also unbind the VAO so other VAO calls won't accidentally modify this
    // VAO, although this rarely happens as it requires glBindVertexArray().
    glBindVertexArray(0);

    // Returning a tuple of the VBO, EBO and VAO.
    return vbo_ebo_vao {VBO, EBO, VAO};
}

