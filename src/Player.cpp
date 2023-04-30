#include<GLUT/glut.h>
#include "../headers/Player.hpp"
#include "../headers/PlatformCube.hpp"
#include "../headers/utilities.hpp"
using namespace std;

Player::Player(GLfloat x,GLfloat y,GLfloat z,GLfloat size,GLuint texture,std::unordered_map<std::pair<int,int>,PlatformCube*,hash_pair>*Platform){
    this->x=x;
    this->y=y;
    this->z=z;
    this->size=size;
    this->texture=texture;
    this->inFall=false;
    this->inMovement=false;
    this->orientation=VERTICAL;
    this->Platform=Platform;
    render(0);
}
void Player::render(float delta){
    glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, texture);
        glTranslatef(x, y, z);
        checkFall(delta);
        this->movement(delta);
        this->orient();
        glScalef(size / 2, size / 2, size / 2);
        drawCuboid();
        glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();
}

void Player::key_detect(int ch, int x, int y){
    if(inMovement || inFall){
        return;
    }
    else{
        inMovement=true;
        angle=0;
    }
    if(ch==GLUT_KEY_UP){
        this->m=BACK;
    }
    if(ch==GLUT_KEY_DOWN){
        this->m=FRONT;
    }
    if(ch==GLUT_KEY_RIGHT){
        this->m=RIGHT;
    }
    if(ch==GLUT_KEY_LEFT){
        this->m=LEFT;
    }
}


void Player::orient(){
    if(orientation==HORIZONTAL_X){
        glTranslatef(size/2,-size/2,0);
        glRotatef(90,0,0,1);
    }
    else if(orientation==HORIZONTAL_Z){
        glTranslatef(0,-size/2,-size/2);
        glRotatef(90,1,0,0);
    }
}

void Player::movement(float delta){
    if(inFall)
        return;
    if(!inMovement)
        return;
    
    if(orientation==VERTICAL){
        if(m==RIGHT){
            angle-=speed*delta;
            if(angle<=-90){
                angle=-90;
                inMovement=false;
                orientation=HORIZONTAL_X;
                x+=size;
                glTranslatef(size,0,0);
                return;
            }
            glTranslatef(size/2,-size,0);
            glRotatef(angle,0,0,1);
            glTranslatef(-size/2,size,0);
        }
        if(m==LEFT){
            angle+=speed*delta;
            if(angle>=90){
                angle=90;
                inMovement=false;
                orientation=HORIZONTAL_X;
                x-=2*size;
                glTranslatef(-2*size,0,0);
                return;
            }
            glTranslatef(-size/2,-size,0);
            glRotatef(angle,0,0,1);
            glTranslatef(size/2,size,0);
        }
        if(m==FRONT){
            angle+=speed*delta;
            if(angle>=90){
                angle=90;
                inMovement=false;
                orientation=HORIZONTAL_Z;
                z+=2*size;
                glTranslatef(0,0,2*size);
                return;
            }
            glTranslatef(0,-size,size/2);
            glRotatef(angle,1,0,0);
            glTranslatef(0,size,-size/2);
        }
        if(m==BACK){
            angle-=speed*delta;
            if(angle<=-90){
                angle=-90;
                inMovement=false;
                orientation=HORIZONTAL_Z;
                z-=size;
                glTranslatef(0,0,-size);
                return;
            }
            glTranslatef(0,-size,-size/2);
            glRotatef(angle,1,0,0);
            glTranslatef(0,size,size/2);
        }
    }
    else if(orientation==HORIZONTAL_X){
        if(m==RIGHT){
            angle-=speed*delta;
            if(angle<=-90){
                angle=-90;
                inMovement=false;
                orientation=VERTICAL;
                x+=2*size;
                glTranslatef(2*size,0,0);
                return;
            }
            glTranslatef(size+size/2,-size,0);
            glRotatef(angle,0,0,1);
            glTranslatef(-size-size/2,size,0);
        }
        if(m==LEFT){
            angle+=speed*delta;
            if(angle>=90){
                angle=90;
                inMovement=false;
                orientation=VERTICAL;
                x-=size;
                glTranslatef(-size,0,0);
                return;
            }
            glTranslatef(-size+size/2,-size,0);
            glRotatef(angle,0,0,1);
            glTranslatef(+size-size/2,size,0);
        }
        if(m==FRONT){
            angle+=speed*delta;
            if(angle>=90){
                angle=90;
                inMovement=false;
                z+=size;
                glTranslatef(0,0,size);
                return;
            }
            glTranslatef(0,-size,size/2);
            glRotatef(angle,1,0,0);
            glTranslatef(0,size,-size/2);
        }
        if(m==BACK){
            angle-=speed*delta;
            if(angle<=-90){
                angle=-90;
                inMovement=false;
                z-=size;
                glTranslatef(0,0,-size);
                return;
            }
            glTranslatef(0,-size,-size/2);
            glRotatef(angle,1,0,0);
            glTranslatef(0,size,size/2);
        }
    }
    else if(orientation==HORIZONTAL_Z){
        if(m==RIGHT){
            angle-=speed*delta;
            if(angle<=-90){
                angle=-90;
                inMovement=false;
                x+=size;
                glTranslatef(size,0,0);
                return;
            }
            glTranslatef(size/2,-size,size+size/2);
            glRotatef(angle,0,0,1);
            glTranslatef(-size/2,size,-size-size/2);
        }
        if(m==LEFT){
            angle+=speed*delta;
            if(angle>=90){
                angle=90;
                inMovement=false;
                x-=size;
                glTranslatef(-size,0,0);
                return;
            }
            glTranslatef(-size/2,-size,-size+size/2);
            glRotatef(angle,0,0,1);
            glTranslatef(size/2,size,size-size/2);
        }
        if(m==FRONT){
            angle+=speed*delta;
            if(angle>=90){
                angle=90;
                inMovement=false;
                orientation=VERTICAL;
                z+=size;
                glTranslatef(0,0,size);
                return;
            }
            glTranslatef(0,-size,size/2);
            glRotatef(angle,1,0,0);
            glTranslatef(0,size,-size/2);
        }
        if(m==BACK){
            angle-=speed*delta;
            if(angle<=-90){
                angle=-90;
                inMovement=false;
                orientation=VERTICAL;
                z-=2*size;
                glTranslatef(0,0,-2*size);
                return;
            }
            glTranslatef(0,-size,-2*size+size/2);
            glRotatef(angle,1,0,0);
            glTranslatef(0,size,2*size-size/2);
        }
    }
}

void Player::checkFall(float delta){
    if(Platform->empty())
        return;
    if(y<=bottomLimit)
        return;
    if(inFall){
        // if(orientation==VERTICAL){
            y-=fallSpeed*delta;
            if(y<=bottomLimit){
                inFall=false;
            }
        // }
        return;
    }
    if(orientation==VERTICAL)
    {
        if(Platform->find({x,z})==Platform->end())
        {
            inFall=true;
        }
    }
    else if(orientation==HORIZONTAL_X)
    {
        if(Platform->find({x,z})==Platform->end() ||Platform->find({x+size,z})==Platform->end() )
        {
            inFall=true;
        }
    }
    else
    {
        if(Platform->find({x,z})==Platform->end()||Platform->find({x,z-size})==Platform->end())
        {
            inFall=true;
        }
    }
}