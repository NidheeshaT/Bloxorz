#include "../headers/utilities.hpp"
#include "../headers/PlatformCube.hpp"
PlatformCube::PlatformCube(GLfloat x,GLfloat y,GLfloat z,GLfloat size,GLuint texture){
    this->x = x;
    this->y = y;
    this->z = z;
    this->size = size;
    this->texture = texture;
    render();
}

void PlatformCube::render(){
    glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, texture);
        glTranslatef(x, y, z);
        glScalef(size / 2, size / 2, size / 2);
        drawCube();
        glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();
}