#ifndef PLATFORMCUBE_H
#define PLATFORMCUBE_H
#include <GLUT/glut.h>
class PlatformCube
{
public:
    GLfloat x, y, z;
    GLfloat size;
    GLuint texture;
    PlatformCube(GLfloat x, GLfloat y, GLfloat z, GLfloat size, GLuint texture);
    void render();
};

#endif