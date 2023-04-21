#ifndef PLAYER_H
#define PLAYER_H
#include <GLUT/glut.h>
class Player
{
public:
    GLfloat x, y, z;
    GLfloat size;
    GLuint texture;
    bool inMovement;
    bool inFall;
    float angle=0.0;
    int speed=180;

    enum Orientation{VERTICAL,HORIZONTAL_Z,HORIZONTAL_X};
    Orientation orientation;
    enum Move{LEFT,RIGHT,FRONT,BACK};
    Move m;

    Player(GLfloat x, GLfloat y, GLfloat z, GLfloat size, GLuint texture);
    void render(float delta);
    void key_detect(int ch, int x, int y);
    void orient();
    void movement(float delta);

};

#endif