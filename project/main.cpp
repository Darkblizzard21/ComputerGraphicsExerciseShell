#include <iostream>


#include <glad/glad.h> 
#include <GLFW/glfw3.h>

// frame buffer size callbak is needed to resize the window
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // update glViewport to new window size
}

int main()
{
    std::cout << "Hello Graphics!" << std::endl;

    // Init GLFW Library and set opengl version and extension hints

    // Create a Window  
    
    // Handle Window creation failure

    // set newly created window to current window in glfw context

    // use glad to load opengl function pointers

    // set open gl view port 
    // 
    // set up resize callback


    // vertex shader source (as string)
    const char* vertexShaderSource =
        "#version 330 core\n"
        "layout(location = 0) in vec3 aPos;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\n";


    // create a shader object for the vertex shader

    // set shader source and complie shader
    
    // check for sucess after compiling

    // fragment shader source (as string)
    const char* fragmentShaderSource =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\n";

    // create and compile fragment shader (analog to vertex shader)

    // check for sucess after compiling

    // link vertex and fragement shader together into a shader program
   

    // set the shader program to be currently used by opengl

    // triangle geometry in Normalized Device Coordinates (NDC)
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    // create a vertex buffer object to hold the tirangle geomtry

    // create an vertex attribute object
    // this holds a reference to the vertex buffer and addtional information how to read the data in the buffer
  
    // 1. bind Vertex Array Object
  
    // 2. copy our vertices array in a buffer for OpenGL to use

    // 3. then set our vertex attributes pointers

    // render loop
    while (true)
    {
        // if esc is pressed - exit application

        // set clear framebuffer with abitrary color 
        // first set clear color and then clear color buffer

        // ======== Rendering ==========
        // set current shader
        
        // bind current vertex attribute object (geometry)
        
        // issue a draw command to the gpu

        // ======== ADVANCE TO NEXT FRAME ==========
        // swap buffers to present the rendered image

        // pool events to recive updates such as key down event or resizes
    }

    // terminate glfw to enshure clean shutdown

    return 0;
}
