#include "Rasterizer.hpp"

static int window_width = Window::width, window_height = Window::height;
static float* pixels = new float[window_width * window_height * 3];

OBJObject bunny1("/Users/Simon/Desktop/ucsd/cse/cse167/bunny.obj");
OBJObject bear1("/Users/Simon/Desktop/ucsd/cse/cse167/bear.obj");
OBJObject dragon1("/Users/Simon/Desktop/ucsd/cse/cse167/dragon.obj");
OBJObject *obj1 = &bunny1;

using namespace std;

struct Color    // generic color class
{
    float r, g, b;  // red, green, blue
};

// Clear frame buffer
void Rastersizer::clearBuffer()
{
    Color clearColor = { 0.0, 0.0, 0.0 };   // clear color: black
    for (int i = 0; i<window_width*window_height; ++i)
    {
        pixels[i * 3] = clearColor.r;
        pixels[i * 3 + 1] = clearColor.g;
        pixels[i * 3 + 2] = clearColor.b;
    }
}

// Draw a point into the frame buffer
void Rastersizer::drawPoint(int x, int y, float r, float g, float b)
{
    if(x < 0 || x > 639 || y < 0 || y > 479)
    {
        return;
    }
    int offset = y*window_width * 3 + x * 3;
    pixels[offset] = r;
    pixels[offset + 1] = g;
    pixels[offset + 2] = b;
    
}

void Rastersizer::rasterize()
{
    //clearBuffer();
    
    glm::mat4 image_space;
    
    glm::mat4 projection_space;
    glm::mat4 camera_space;
    glm::vec4 p;
    glm::vec4 result;
    for(int i = 0; i < obj1->vertices.size(); i++)
    {
        
        //vector p
        p = glm::vec4(obj1->vertices[i].x, obj1->vertices[i].y, obj1->vertices[i].z, 1.0f);
        
        //image_space
        image_space[0] = glm::vec4((window_width-0.0f)/2.0f, 0.0f, 0.0f, 0.0f);
        image_space[1] = glm::vec4(0.0f, (window_height-0.0f)/2.0f, 0.0f, 0.0f);
        image_space[2] = glm::vec4(0.0f, 0.0f, 1.0f/2.0f, 1.0f/2.0f);
        image_space[3] = glm::vec4( (window_width+0.0f)/2.0f, (window_height+0.0f)/2.0f, 1.0f/2.0f, 1.0f);
        
        //camera_space
        camera_space = glm::lookAt(glm::vec3(0.0f, 0.0f, 20.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        projection_space = glm::perspective(glm::radians(60.0f),(float)window_width / (float)window_height, 1.0f, 1000.0f);
        result =   image_space * projection_space * camera_space * obj1->toWorld * p;
        drawPoint((int)(result[0]/result[3]), (int)(result[1]/result[3]), obj1->normals[i].x, obj1->normals[i].y, obj1->normals[i].z);
    }
    
}

// Called whenever the window size changes
void Rastersizer::resizeCallback(GLFWwindow* window, int width, int height)
{
    window_width = width;
    window_height = height;
    delete[] pixels;
    pixels = new float[window_width * window_height * 3];
}

void Rastersizer::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Check for a key press
    if (action == GLFW_PRESS /* TODO REMOVE FROM STARTER */ || action == GLFW_REPEAT)
    {
        // Check if escape was pressed
        if (key == GLFW_KEY_ESCAPE)
        {
            // Close the window. This causes the program to also terminate.
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
    }
}

void Rastersizer::displayCallback(GLFWwindow* window)
{
    clearBuffer();
    rasterize();
    
    // glDrawPixels writes a block of pixels to the framebuffer
    glDrawPixels(window_width, window_height, GL_RGB, GL_FLOAT, pixels);
    
    // Gets events, including input such as keyboard and mouse or window resizing
    glfwPollEvents();
    // Swap buffers
    glfwSwapBuffers(window);
}

void Rastersizer::errorCallback(int error, const char* description)
{
    // Print error
    fputs(description, stderr);
}

/*int main() {
    // Initialize GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }
    
    // 4x antialiasing
    glfwWindowHint(GLFW_SAMPLES, 4);
    
    // Create the GLFW window
    GLFWwindow* window = glfwCreateWindow(window_height, window_height, "Rastizer", NULL, NULL);
    
    // Check if the window could not be created
    if (!window)
    {
        fprintf(stderr, "Failed to open GLFW window.\n");
        glfwTerminate();
        return -1;
    }
    
    // Make the context of the window
    glfwMakeContextCurrent(window);
    
    // Set swap interval to 1
    glfwSwapInterval(1);
    
    
    // Set the error callback
    glfwSetErrorCallback(errorCallback);
    // Set the key callback
    glfwSetKeyCallback(window, keyCallback);
    // Set the window resize callback
    glfwSetWindowSizeCallback(window, resizeCallback);
    
    // Loop while GLFW window should stay open
    while (!glfwWindowShouldClose(window))
    {
        // Main render display callback. Rendering of objects is done here.
        displayCallback(window);
    }
    
    // Destroy the window
    glfwDestroyWindow(window);
    // Terminate GLFW
    glfwTerminate();
    
    exit(EXIT_SUCCESS);
}*/