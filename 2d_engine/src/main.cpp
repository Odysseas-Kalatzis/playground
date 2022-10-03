#include <iostream>
#include <cstdio>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "include/rectangle.h"
#include "include/shader.h"
#include "include/texture_load.h"

// Used by glfwSetFramebufferSizeCallback()
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// Used to process keyboard and mouse input.
void processInput(GLFWwindow* window);


// The window width and height.
static int window_width;
static int window_height;
// The projection matrix used.
static glm::mat4 projection;

int main()
{

    // Using GLFW to create our application window.
    // Setting GLFW to use OpenGL of at least 3.3 version.
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // GLFW window creation.
    auto monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
    window_width = mode->width;
    window_height = mode->height;

    // Set default size to be half the screen width and height.
    GLFWwindow* window = glfwCreateWindow(window_width*0.8, window_height*0.8, 
            "Application", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    // Telling GLFW to make the context of window the main
    // context on the current thread.
    glfwMakeContextCurrent(window);

    // Telling OpenGL the size of the rendering window.
    // When the user resizes a window, the viewport is updated.
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // GLAD manages function pointers for OpenGL so we want to initialize GLAD 
    // before we call any OpenGL function: 
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout << "Failed to initialise GLAD\n";
        return -1;
    }


    // Uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Creating the shader program, VBO and VAO for the triangle.
    Shader shaderProgram("../shaders/shader.vert", "../shaders/shader.frag");

    // Load texture.
    Texture_Load tex_load_1("../assets/container.jpg");
    Texture_Load tex_load_2("../assets/awesomeface.png", 1);

    // We also have to tell OpenGL to which texture unit each shader sampler belongs 
    // to by setting each sampler using glUniform1i. We only need to do this once.
    shaderProgram.use(); // First we need to bind shader before setting its uniforms.
    shaderProgram.setInt("texture1", 0);
    shaderProgram.setInt("texture2", 1);

    
    // Creating the projection matrix.
    //projection = glm::perspective(glm::radians(70.0f), 
            //static_cast<float>(window_width)/static_cast<float>(window_height),
            //0.1f, 10000.0f);

    //projection = glm::ortho(0.0f, static_cast<float>(window_width), 
            //static_cast<float>(window_height), 0.0f, -1.0f, 1.0f); 

    projection = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    //view = glm::translate(view, glm::vec3(0.0f, 0.0f, 3.0f));

    glm::mat4 model = glm::mat4(1.0f);
    //model = glm::rotate(model, glm::radians(-55.0f), 
            //glm::vec3(1.0f, 0.0, 0.0));

    vbo_ebo_vao data_struct = makeRectangle();

    GLuint VBO = data_struct.vbo;
    GLuint EBO = data_struct.ebo;
    GLuint VAO = data_struct.vao;

    // Render loop to keep the window running.
    while (!glfwWindowShouldClose(window))
    {
        // Handle input.
        processInput(window);

        // Render:

        // At the start of each frame we want to clear the screen, 
        // otherwise we would still see results from previous frame.
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        // More buffer bits to clear include GL_DEPTH_BUFFER_BIT
        // and GL_STENCIL_BUFFER_BIT.
        glClear(GL_COLOR_BUFFER_BIT);

        // ------------------------------  
        
        // Bind texture for use.
        tex_load_1.use();
        tex_load_2.use();

        // Set uniform matrices to shaderProgram.
        shaderProgram.setMat4f("projection", glm::value_ptr(projection));
        shaderProgram.setMat4f("view", glm::value_ptr(view));
        shaderProgram.setMat4f("model", glm::value_ptr(model));

        // Draw triangle.
        shaderProgram.use();

        glBindVertexArray(VAO);

        // First parameter represents the mode we wish to draw.
        // Second parameter represents the number of indices of our EBO.
        // Third parameter is the type of the indices and 4rth parameter is offset.
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // glBindVertexArray(0); // No Need to unbind every time.

        // ------------------------------  

        // Swaps the colour buffer used to render.
        glfwSwapBuffers(window);
        // Checks if any events are triggered, like keyboard input,
        // mouse events, etc... It then calls corresponding functions,
        // which we can register via callback methods.
        glfwPollEvents();
    }
    
    // De-Allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    shaderProgram.destroy();

    // When we exit the render loop, we clean all of GLFW's resources
    // that were allocated.
    glfwTerminate();
    return 0;
}

// Process all input. Query GLFW whether relevant keys are
// pressed/released this frame and react accordingly.
void processInput(GLFWwindow* window)
{
    // Close window when esc key is pressed.
    // If it is not pressed, glfwGetKey returns GLFW_RELEASE.
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

// Function used as a callback function to resize
// viewport when window size is altered. 
void framebuffer_size_callback([[maybe_unused]] GLFWwindow* window, 
        int width, int height)
{
    window_width = width;
    window_height = height;

    // Update the projection matrix.
    projection = glm::perspective(glm::radians(70.0f), 
            static_cast<float>(window_width)/static_cast<float>(window_height),
            0.1f, 10000.0f);

    // First two parameters set the location of the lower left corner of the window.
    // Note that width and height will be significantly larger
    // than specified on retina displays.
    glViewport(0, 0, window_width, window_height);
    std::cout << "Window size changed: Width = " << width << ", Height = " 
        << height << '\n';
}

