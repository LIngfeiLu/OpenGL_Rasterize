#include "OBJObject.h"
using namespace std;
OBJObject::OBJObject(const char *filepath)
{
    toWorld = glm::mat4(1.0f);
    parse(filepath);
    angle = 1.0f;
}

void OBJObject::parse(const char *filepath)
{
    //TODO parse the OBJ file
    // Populate the face indices, vertices, and normals vectors with the OBJ Object data
    ifstream input(filepath);
    float x,y,z;
    char c1, c2;
    string buffer;
    
    while(1)
    {
        input.get(c1);
        input.get(c2);
        
        if(input.eof())
            break;
        
        if(c1 == 'v' && c2 == ' ')
        {
            input >> x >> y >> z;
            vertices.push_back(glm::vec3(x,y,z));
        }
        
        if(c1 == 'v' && c2 == 'n')
        {
            input >> x >> y >> z;
            normals.push_back(glm::vec3(x,y,z));
        }
        
        getline(input, buffer);
        
    }
}

void OBJObject::draw()
{
    glMatrixMode(GL_MODELVIEW);
    
    // Push a save state onto the matrix stack, and multiply in the toWorld matrix
    glPushMatrix();
    glMultMatrixf(&(toWorld[0][0]));
    
    glBegin(GL_POINTS);
    
    // Loop through all the vertices of this OBJ Object and render them
    for (unsigned int i = 0; i < vertices.size(); ++i)
    {
        glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
        glm::vec3 color = normalize(normals[i]);
        float x = (color.x+1.0f)/2.0f;
        float y = (color.y+1.0f)/2.0f;
        float z = (color.z+1.0f)/2.0f;
        glColor3f(x, y, z);
    }
    glEnd();
    
    // Pop the save state off the matrix stack
    // This will undo the multiply we did earlier
    glPopMatrix();
}

void OBJObject::update()
{
    spin();
}

void OBJObject::spin()
{
    if (this->angle > 360.0f || this->angle < -360.0f) this->angle = 0.0f;
    // This creates the matrix to rotate the cube
    this->toWorld = glm::rotate(this->toWorld, this->angle / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
}

void OBJObject::translation(int mode)
{
    //move left
    if(mode == 0)
        this->toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, 0.0f)) * this->toWorld;
    //move right
    if(mode == 1)
        this->toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * this->toWorld;
    //move down
    if(mode == 2)
        this->toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 0.0f)) * this->toWorld;
    //move up
    if(mode == 3)
        this->toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * this->toWorld;
    //move into
    if(mode == 5)
        this->toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 1.0f)) * this->toWorld;
    //move out
    if(mode == 4)
        this->toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.0f)) * this->toWorld;
}

void OBJObject::scale(int mode)
{
    //scale down
    if(mode == 0)
        this->toWorld = glm::scale(toWorld, glm::vec3(0.5f, 0.5f, 0.5f));
    //scale up
    if(mode == 1)
        this->toWorld = glm::scale(toWorld, glm::vec3(2.0f, 2.0f, 2.0f));
}

void OBJObject::orbit(int mode)
{
    //counterclockwise
    if(mode == 0)
    {
        if (this->angle > 360.0f || this->angle < -360.0f) this->angle = 0.0f;
        // This creates the matrix to rotate the cube
        this->toWorld = glm::rotate(glm::mat4(), 0.2f, glm::vec3(0.0f, 0.0f, 1.0f)) * this->toWorld;
    }
        
    //clockwise
    if(mode == 1){
        if (this->angle > 360.0f || this->angle < -360.0f) this->angle = 0.0f;
    // This creates the matrix to rotate the cube
    this->toWorld = glm::rotate(glm::mat4(), -0.2f, glm::vec3(0.0f, 0.0f, 1.0f)) * this->toWorld;
    }
}


