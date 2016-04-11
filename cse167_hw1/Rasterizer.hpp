#include <math.h>
#include "OBJObject.h"
#include "Window.h"

using namespace std;

class Rastersizer
{
public:
    static void clearBuffer();
    static void drawPoint(int x, int y, float r, float g, float b);
    static void rasterize();
    static void resizeCallback(GLFWwindow* window, int width, int height);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void displayCallback(GLFWwindow* window);
    static void errorCallback(int error, const char* description);
};