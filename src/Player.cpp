#include<GLUT/glut.h>
#include "../headers/Player.hpp"
#include "../headers/utilities.hpp"

Player::Player(GLfloat x,GLfloat y,GLfloat z,GLfloat size,GLuint texture){
    this->x=x;
    this->y=y;
    this->z=z;
    this->size=size;
    this->texture=texture;
    this->inFall=false;
    this->inMovement=false;
    this->orientation=VERTICAL;
    render(0);
}

void Player::render(float delta){
    glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, texture);
        glTranslatef(x, y, z);
        this->orient();
        this->movement(delta);
        glScalef(size / 2, size / 2, size / 2);
        drawCuboid();
        glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();
}

void Player::key_detect(int ch, int x, int y){
    if(inMovement){
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
            }
            glTranslatef(0,-size,-size/2);
            glRotatef(angle,1,0,0);
            glTranslatef(0,size,size/2);
        }
    }
    // else if(orientation==HORIZONTAL_X){
    //     if(m==RIGHT){
    //         angle-=speed*delta;
    //         if(angle<=-90){
    //             angle=-90;
    //             inMovement=false;
    //             orientation=HORIZONTAL_X;
    //             x+=size;
    //         }
    //         glTranslatef(3*size/2,-size/2,0);
    //         glRotatef(angle,0,0,1);
    //         glTranslatef(-3*size/2,size/2,0);
    //     }
    // }
}