#pragma once

// Returns VBO, EBO and VAO.
struct vbo_ebo_vao
{
    unsigned int vbo;
    unsigned int ebo;
    unsigned int vao;
};

vbo_ebo_vao makeRectangle();

