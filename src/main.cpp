#include "GLUT/glut.h"
#include<iostream>
#include<math.h>
#include "../headers/utilities.hpp"

using namespace std;
float x=0;
float z=60;
float y=0;
int angle=0;

GLuint texture;

void light(){
    float diffuse[]={0,1,0,1};
    float ambient[]={0.1,0.1,0.1,1};
    float specular[]={1,0.1,0.1,1};
    float position[]={0,0,60,1};
    float disable[]={0,0,0,1};
        glShadeModel(GL_SMOOTH);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuse);
    glLightfv(GL_LIGHT0,GL_AMBIENT,ambient);
    glLightfv(GL_LIGHT0,GL_SPECULAR,specular);
    glLightfv(GL_LIGHT0,GL_POSITION,position);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}
void material_white(){
    float white[]={1,1,1,1};
    float disable[]={0,0,0,0};
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,white);
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,120.0f);
}
void init(){
    glClearColor(0.0, 0.0 ,0.0, 0.0);
    light();
    material_white();
    // glColorMaterial(GL_FRONT_AND_BACK,GL_SPECULAR|GL_AMBIENT_AND_DIFFUSE);

    // glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_NORMALIZE);
    texture=loadBMPTexture("download.bmp");
    // texture=loadtextures("water.raw",256,256);
}
void display()
{
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(x,y,z,0,0,0,0,1,0);
    // glBindTexture(GL_TEXTURE_2D,texture);
    glPushMatrix();
        glTranslatef(0,-40,0);
        glRotatef(angle,0,1,0);
        drawCube(0,0,0,20);
    glPopMatrix();
    glutSwapBuffers();
}
void reshape(int w,int h)
{
    int x=(w>h?(w-min(w,h))/2:0);
    int y=(h>w?(h-min(w,h))/2:0);
    glViewport(x,y,min(w,h),min(w,h));
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // gluOrtho2D(-2,2,-2,2);
    // glOrtho(-30,30,-30,30,0,1000);
    // gluPerspective(45,1,5,x);
    glFrustum(-30,30,-30,30,10,100);
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
        y++;
    else if(ch=='s')
        y--;

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



