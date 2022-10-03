#version 330 core
layout (location = 0) in vec3 aPos; // Position variable has attribute 0.
layout (location = 1) in vec3 aColor; // Color variable has attribute 1.
layout (location = 2) in vec2 aTexCoord; // Texture coordinates with attribute 2.

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 ourColor; // Output a color to the gragment shader.
out vec2 TexCoord; // Pass texture coordinates to fragment (this interpolates them).

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    // Set ourColor to the input color we got from vertex data.
    ourColor = aColor; 

    TexCoord = aTexCoord;
}

