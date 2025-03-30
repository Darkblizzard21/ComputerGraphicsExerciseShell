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

    // render loop
    while (true)
    {
        // if esc is pressed - exit application

        // set clear framebuffer with abitrary color 
        // first set clear color and then clear color buffer


        // swap buffers to present the rendered image

        // pool events to recive updates such as key down event or resizes
    }

    // terminate glfw to enshure clean shutdown

    return 0;
}
