#ifndef OBJOBJECT_H
#define OBJOBJECT_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>

class OBJObject
{
public:
    std::vector<unsigned int> indices;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    glm::mat4 toWorld;
    OBJObject(const char* filepath);
    
    float angle;
	void parse(const char* filepath);
	void draw();
    void update();
    void spin();
    void translation(int mode);
    void scale(int mode);
    void orbit(int mode);
};

#endif