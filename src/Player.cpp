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
    this->level=1;
    render(0);
}
void Player::render(float delta){
    glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, texture);
        glTranslatef(x, y, z);
        checkTarget(delta);
        checkFall(delta);
        this->movement(delta);
        this->orient();
        glScalef(size / 2, size / 2, size / 2);
        drawCuboid();
        glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();
}

void Player::key_detect(int ch, int x, int y){
    if(inMovement || inFall ||inTarget){
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
    if(inFall||inTarget||!inMovement)
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
    if(inTarget)
        return;
    if(Platform->empty())
        return;
    if(y<=bottomLimit)
        return;
    if(inFall){
        if(orientation==VERTICAL){
            y-=fallSpeed*delta;
            if(y<=bottomLimit){
                inFall=false;
                //gameover 
            }
        }
        else if(orientation==HORIZONTAL_X){
            bool left=Platform->find({x,z})==Platform->end();
            bool right=Platform->find({x+size,z})==Platform->end();

            if(left && right){
                y-=fallSpeed*delta;
                //gameover
            }
            else if(left){
                angle+=speed*delta;
                if(angle>=90){
                    angle=90;   
                    orientation=VERTICAL;
                    y-=size;
                    glTranslatef(0,-size,0);
                    inFall=false;
                    return;
                }
                glTranslatef(+size/2,-size,-size+size/2);
                glRotatef(angle,0,0,1);
                glTranslatef(-size/2,size,size-size/2);
            }
            else if(right){
                angle-=speed*delta;
                if(angle<=-90){
                    angle=-90;   
                    orientation=VERTICAL;
                    y-=size;
                    x+=size;
                    glTranslatef(size,-size,0);
                    inFall=false;
                    return;
                }
                glTranslatef(+size/2,-size,-size+size/2);
                glRotatef(angle,0,0,1);
                glTranslatef(-size/2,size,size-size/2);
            }
            if(y<=bottomLimit)
                inFall=false;
        }
        else if(orientation==HORIZONTAL_Z){
            bool front=Platform->find({x,z})==Platform->end();
            bool back=Platform->find({x,z-size})==Platform->end();
            if(front && back){
                y-=fallSpeed*delta;
                //gameover
            }
            else if(front){
                angle+=speed*delta;
                if(angle>=90){
                    angle=90;   
                    orientation=VERTICAL;
                    y-=size;
                    glTranslatef(0,-size,0);
                    inFall=false;
                    return;
                }
                glTranslatef(0,-size,-size/2);
                glRotatef(angle,1,0,0);
                glTranslatef(0,size,+size/2);
            }
            else if(back){
                angle-=speed*delta;
                if(angle<=-90){
                    angle=-90;   
                    orientation=VERTICAL;
                    y-=size;
                    z-=size;
                    glTranslatef(0,-size,-size);
                    inFall=false;
                    return;
                }
                glTranslatef(0,-size,-size/2);
                glRotatef(angle,1,0,0);
                glTranslatef(0,size,+size/2);
            }
            if(y<=bottomLimit)
                inFall=false;
        }
        return;
    }


    //to check if player is in falling positions
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
            angle=0;
        }
    }
    else
    {
        if(Platform->find({x,z})==Platform->end()||Platform->find({x,z-size})==Platform->end())
        {
            inFall=true;
            angle=0;
        }
    }
}

void Player::checkTarget(float delta){
    if(inTarget){
        angle+=200*delta;
        if(angle>=360)
            angle=0;
        
        glTranslatef(0,size/4 + abs(int(angle)-180)/180.0*size/2,0);
        glRotatef(angle,0,1,0);
        return;
    }
    if(orientation==VERTICAL && targetX==x && targetZ==z){
        inTarget=true;
        angle=0;
    }
}