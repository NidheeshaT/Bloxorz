#ifndef PLAYER_H
#define PLAYER_H
#include <GLUT/glut.h>
class Player
{
public:
    GLfloat x, y, z;
    GLfloat size;
    GLuint texture;

    Player(GLfloat x, GLfloat y, GLfloat z, GLfloat size, GLuint texture);
    void render();
};

#endif