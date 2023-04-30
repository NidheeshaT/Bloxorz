#ifndef PLAYER_H
#define PLAYER_H
#include<unordered_map>
#include "../headers/PlatformCube.hpp"
#include "../headers/utilities.hpp"
#include <GLUT/glut.h>
class Player
{
public:
    GLfloat x, y, z;
    GLfloat size;
    GLuint texture;
    bool inMovement;
    bool inFall;
    bool inTarget;
    float angle=0.0;
    int speed=180;
    int fallSpeed=100;
    float bottomLimit=-300;
    int targetX;
    int targetZ;
    std::unordered_map<std::pair<int,int>,PlatformCube*,hash_pair>*Platform;


    enum Orientation{VERTICAL,HORIZONTAL_Z,HORIZONTAL_X};
    Orientation orientation;
    enum Move{LEFT,RIGHT,FRONT,BACK};
    Move m;

    Player(GLfloat x, GLfloat y, GLfloat z, GLfloat size, GLuint texture,std::unordered_map<std::pair<int,int>,PlatformCube*,hash_pair>*Platform);
    void render(float delta);
    void key_detect(int ch, int x, int y);
    void orient();
    void movement(float delta);
    void checkFall(float delta);
    void checkTarget(float delta);

};

#endif