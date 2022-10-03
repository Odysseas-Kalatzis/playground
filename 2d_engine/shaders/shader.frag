#version 330 core
out vec4 FragColor;
in vec3 ourColor; // Passed from the vertex shader (interpolated).
in vec2 TexCoord; // Passed from the vertex shader (interpolated).

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    vec4 mixed_col = mix(texture(texture1, TexCoord), 
        texture(texture2, TexCoord), 0.5);
    FragColor = 0.5*mixed_col + 0.5*vec4(ourColor, 1.0);  
    //FragColor = texture(texture1, TexCoord) * vec4(ourColor, 1.0);  
    //FragColor = vec4(ourColor, 1.0);
}

