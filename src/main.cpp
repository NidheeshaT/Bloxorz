#include "GLUT/glut.h"
#include<iostream>
#include<math.h>
#include "../headers/utilities.hpp"

using namespace std;
float x=0;
int z=50;
int angle=0;

GLuint texture;

void init(){
    glClearColor(0.0, 0.0 ,0.0, 0.0);
    float f[]={.7,0.7,0.7,1.0};
    float f2[]={0.1,0.1,0.1,1};
    float p[]={60,0,0,0};
    float disable[]={0,0,0,1};
    float white[]={1,1,1,1};
    glShadeModel(GL_SMOOTH);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,f);
     glLightfv(GL_LIGHT0,GL_AMBIENT,f2);
    // glLightfv(GL_LIGHT0,GL_SPECULAR,f3);
    glLightfv(GL_LIGHT0,GL_POSITION,p);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
        // glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,white);
        // glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,white);
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,120.0f);
    glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);

    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);
    // glDepthFunc(GL_LEQUAL);
    glEnable(GL_TEXTURE_2D);
    texture=loadtextures("download.rgb",225,225);
}
void display()
{
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(x,0,z,0,0,0,0,1,0);
    // glBindTexture(GL_TEXTURE_2D,texture);
    glPushMatrix();
        glRotatef(angle,0,1,0);
        drawCube(0,0,0,20);
    glPopMatrix();
    glutSwapBuffers();
}
void reshape(int w,int h)
{
    int x=(w>h?w/4:0);
    int y=(h>w?h/4:0);
    glViewport(x,y,min(w,h),min(w,h));
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // gluOrtho2D(-2,2,-2,2);
    // glOrtho(-size,x,-size,x,1,x);
    // gluPerspective(45,1,5,x);
    glFrustum(-30,30,-30,30,20,200);
    glMatrixMode(GL_MODELVIEW);
}

void timer(int t)
{
    glutPostRedisplay();
    glutTimerFunc(1000/60,timer,0);
    // r+=0.5;
    // z-=1;
    angle+=2;
}
void keyboard(unsigned char ch,int,int){
    if(ch=='w')
        z-=1;
    else if(ch=='s')
        z++;

    if(ch=='d')
        x++;
    else if(ch=='a')
        x--;

    // if(ch=='r')
    // {
    //     angle+=1;
    //     z=80*cos(angle*PI/180);
    //     x=80*sin(angle*PI/180);
    //     if(angle==360)
    //         angle=0;
    // }


}
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH|GLUT_MULTISAMPLE);
    glutInitWindowSize(500,500);
    glutCreateWindow("Hello World");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0,timer,0);
    init();
    glutMainLoop();
    return 0;
}



