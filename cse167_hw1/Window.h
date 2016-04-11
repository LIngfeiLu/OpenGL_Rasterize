#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "OBJObject.h"



class Window
{
public:
	static int width;
	static int height;
	static void initialize_objects();
	static void clean_up();
	static GLFWwindow* create_window(int width, int height);
	static void resize_callback(GLFWwindow* window, int width, int height);
    static void resizeCallback(GLFWwindow* window, int width, int height);
	static void idle_callback();
	static void display_callback(GLFWwindow*);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void resize(bool mode);
    static int getState();
    static void clearBuffer();
    static void drawPoint(int x, int y, float r, float g, float b);
    static void rasterize();
    static void displayCallback(GLFWwindow* window);
};

#endif
