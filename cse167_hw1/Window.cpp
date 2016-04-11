#include "Window.h"

using namespace std;

const char* window_title = "GLFW Starter Project";
OBJObject bunny("/Users/Simon/Desktop/ucsd/cse/cse167/bunny.obj");
OBJObject bear("/Users/Simon/Desktop/ucsd/cse/cse167/bear.obj");
OBJObject dragon("/Users/Simon/Desktop/ucsd/cse/cse167/dragon.obj");
OBJObject *obj = &bunny;
int Window::width;
int Window::height;

float size = 1.0f;
int state = -1;
static float* pixels = new float[Window::width * Window::height * 3];
static float* z_buffer = new float();

void Window::initialize_objects()
{
}

void Window::clean_up()
{
}

int Window::getState()
{
    return state;
}

struct Color    // generic color class
{
    float r, g, b;  // red, green, blue
};

// Clear frame buffer
void Window::clearBuffer()
{
    Color clearColor = { 0.0, 0.0, 0.0 };   // clear color: black
    for (int i = 0; i< width * height; ++i)
    {
        pixels[i * 3] = clearColor.r;
        pixels[i * 3 + 1] = clearColor.g;
        pixels[i * 3 + 2] = clearColor.b;
    }
}

// Draw a point into the frame buffer
void Window::drawPoint(int x, int y, float r, float g, float b)
{
    
    if(x < 0 || x >= width || y < 0 || y >= height)
    {
        return;
    }
    
    int offset = y* width * 3 + x * 3;
    pixels[offset] = r;
    pixels[offset + 1] = g;
    pixels[offset + 2] = b;
    
}

void Window::rasterize()
{
    //clearBuffer();
    
    glm::mat4 image_space;
    
    glm::mat4 projection_space;
    glm::mat4 camera_space;
    glm::vec4 p;
    glm::vec4 result;
    for(int i = 0; i < obj->vertices.size(); i++)
    {
        
        //vector p
        p = glm::vec4(obj->vertices[i].x, obj->vertices[i].y, obj->vertices[i].z, 1.0f);
        
        //image_space
        image_space[0] = glm::vec4((width-0.0f)/2.0f, 0.0f, 0.0f, 0.0f);
        image_space[1] = glm::vec4(0.0f, (height-0.0f)/2.0f, 0.0f, 0.0f);
        image_space[2] = glm::vec4(0.0f, 0.0f, 1.0f/2.0f, 0.0f);
        image_space[3] = glm::vec4( (width+0.0f)/2.0f, (height+0.0f)/2.0f, 1.0f/2.0f, 1.0f);
        
        //camera_space
        camera_space = glm::lookAt(glm::vec3(0.0f, 0.0f, 20.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        projection_space = glm::perspective(glm::radians(60.0f),(float)width / (float)height, 1.0f, 1000.0f);
        result =   image_space * projection_space * camera_space * obj->toWorld * p;
        
        for(int j = 0;  j< size; j++)
        {
            for(int k = 0; k < size; k++)
            {
                drawPoint((int)(result[0]/result[3]+j), (int)(result[1]/result[3]+k), obj->normals[i].x, obj->normals[i].y, obj->normals[i].z);
            }
        }
    }
    
}

GLFWwindow* Window::create_window(int width, int height)
{
    cout << "create window"<<width << endl;
    // Initialize GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return NULL;
    }
    
    // 4x antialiasing
    glfwWindowHint(GLFW_SAMPLES, 4);
    
    // Create the GLFW window
    GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);
    
    // Check if the window could not be created
    if (!window)
    {
        fprintf(stderr, "Failed to open GLFW window.\n");
        glfwTerminate();
        return NULL;
    }
    
    
    
    // Make the context of the window
    
    
    glfwMakeContextCurrent(window);
    
    
    // Set swap interval to 1
    glfwSwapInterval(1);
    cout << "begin of create window" <<  width << endl;
    // Call the resize callback to make sure things get drawn immediately
    glfwGetFramebufferSize(window, &width, &height);
    cout << "end of create window"<<width << endl;
    
    Window::resize_callback(window, width, height);
    
    return window;
}

void Window::resize_callback(GLFWwindow* window, int width1, int height1)
{
    width = width1;
    height = height1;
    // Set the viewport size
    glViewport(0, 0, width, height);
    // Set the matrix mode to GL_PROJECTION to determine the proper camera properties
    glMatrixMode(GL_PROJECTION);
    // Load the identity matrix
    glLoadIdentity();
    // Set the perspective of the projection viewing frustum
    gluPerspective(60.0, double(width) / (double)height, 1.0, 1000.0);
    // Move camera back 20 units so that it looks at the origin (or else it's in the origin)
    glTranslatef(0, 0, -20);
}

void Window::resizeCallback(GLFWwindow* window, int width1, int height1)
{
    width = width1;
    height = height1;
    delete[] pixels;
    pixels = new float[width * height * 3];
}

void Window::idle_callback()
{
    // Perform any updates as necessary. Here, we will spin the cube slightly.
    
    obj->update();
}

void Window::display_callback(GLFWwindow* window)
{
    // Clear the color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Set the matrix mode to GL_MODELVIEW
    glMatrixMode(GL_MODELVIEW);
    // Load the identity matrix
    glLoadIdentity();
    
    // Render objects
    obj->draw();
    
    // Gets events, including input such as keyboard and mouse or window resizing
    glfwPollEvents();
    // Swap buffers
    glfwSwapBuffers(window);
}

void Window::displayCallback(GLFWwindow* window)
{
    clearBuffer();
    rasterize();
    
    // glDrawPixels writes a block of pixels to the framebuffer
    glDrawPixels(width,height, GL_RGB, GL_FLOAT, pixels);
    
    // Gets events, including input such as keyboard and mouse or window resizing
    glfwPollEvents();
    // Swap buffers
    glfwSwapBuffers(window);
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Check for a key press
    if (action == GLFW_PRESS)
    {
        // Check if escape was pressed
        if (key == GLFW_KEY_ESCAPE)
        {
            // Close the window. This causes the program to also terminate.
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
        
        else if(key == GLFW_KEY_1)
        {
            obj = &bunny;
        }
        
        else if(key == GLFW_KEY_2)
        {
            obj = &bear;
        }
        
        else if(key == GLFW_KEY_3)
        {
            obj = &dragon;
        }
        
        else if(key == GLFW_KEY_P && mods)
        {
            if(state == -1)
            {
                size = size * 2;
                glPointSize(size);
            }
            
            if(state == 1)
            {
                size += 1;
            }
        }
        
        else if(key == GLFW_KEY_P)
        {
            if(state == -1)
            {
                size = size / 2;
                glPointSize(size);
            }
            
            if(state == 1)
            {
                size -= 1;
                if(size < 1)
                    size = 1;
            }
        }
        
        else if(key == GLFW_KEY_X && mods)
        {
            
            obj->translation(1);
        }
        
        else if(key == GLFW_KEY_X)
        {
            
            obj->translation(0);
        }
        
        else if(key == GLFW_KEY_Y && mods)
        {
            
            obj->translation(3);
        }
        
        else if(key == GLFW_KEY_Y)
        {
            
            obj->translation(2);
        }
        
        else if(key == GLFW_KEY_Z && mods)
        {
            
            obj->translation(5);
        }
        
        else if(key == GLFW_KEY_Z)
        {
            
            obj->translation(4);
        }
        else if(key == GLFW_KEY_S && mods)
        {
            
            obj->scale(1);
        }
        else if(key == GLFW_KEY_S)
        {
            
            obj->scale(0);
        }
        //clockwise
        else if(key == GLFW_KEY_O && mods)
        {
            
            obj->orbit(1);
        }
        
        //counterclockwise
        else if(key == GLFW_KEY_O)
        {
            
            obj->orbit(0);
        }
        
        else if(key == GLFW_KEY_R)
        {
            obj->toWorld = glm::mat4(1.0f);
            if(state == -1)
                glPointSize(1.0f);
            size = 1.0f;
        }
        
        else if(key == GLFW_KEY_T)
        {
            state  = state * -1;
            if(state == -1)
                glPointSize(size);
        }
    }
}
