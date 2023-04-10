#include "GLUT/glut.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include "../headers/utilities.hpp"
#include "../headers/PlatformCube.hpp"
#include "../headers/Player.hpp"
#define PI 3.14159265

using namespace std;
float cameraRadius=200;
int angleX=0;
int angleY=0;
int angle=0;

int windowWidth = 100;
int windowHeight = 100;
int z_near=50;
int z_far=800;
float light_position[] = {0, 300, -600, 1};
float light_color[] = {1, 0.7, 0.2};

GLuint texture,sun;


void camera(float cameraRadius,int angleX,int angleY)
{
    GLdouble cameraX,cameraY, cameraZ;
    cameraY=sin(PI*angleX/180)*cameraRadius;
    cameraZ=cos(PI*angleX/180)*cameraRadius;

    cameraX=sin(PI*angleY/180)*cameraZ;
    cameraZ=cos(PI*angleY/180)*cameraZ;
    gluLookAt(cameraX, cameraY, cameraZ, 0, 0, 0,0, cos(PI*angleX/180), 0);
}
void material_white();
void material_emissive();
// 1. set up the lighting in the scene
void light(float position[4],float color[3],float diffuse_strength,float ambient_strength,float specular_strength)
{
    // glDisable(GL_DEPTH_TEST);
    float diffuse[] = {color[0] * diffuse_strength, color[1] * diffuse_strength, color[2] * diffuse_strength, 1};
    float ambient[] = {color[0] * ambient_strength, color[1] * ambient_strength, color[2] * ambient_strength, 1};
    float specular[] = {color[0] * specular_strength, color[1] * specular_strength, color[2] * specular_strength, 1};
    glShadeModel(GL_SMOOTH);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glPushMatrix();
        glTranslatef(position[0], position[1], position[2]);
        material_emissive();
        glBindTexture(GL_TEXTURE_2D, sun);
    GLUquadricObj *sphere = gluNewQuadric();
        gluQuadricTexture(sphere, GL_TRUE);
        gluQuadricNormals(sphere, GLU_SMOOTH);
        gluSphere(sphere, 100.0, 50, 50);
        gluDeleteQuadric(sphere);
    glPopMatrix();
    // glEnable(GL_DEPTH_TEST);
}

// 2. set up the material properties for objects in the scene
void material_white()
{
    float white[] = {1, 1, 1, 1};
    float disable[] = {0, 0, 0, 0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, white);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, white);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 120.0f);
    glMaterialfv(GL_FRONT, GL_EMISSION, disable);
}

void material_emissive()
{
    float disable[] = {0, 0, 0, 0};
    GLfloat mat_emission[] = { 0.6f, 0.4784f, 0.0f, 1.0 };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, disable);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, disable);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, disable);
    glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
}
void material_emissive_white()
{
    float disable[] = {0, 0, 0, 0};
    GLfloat mat_emission[] = { 1, 0.8, 2, 1.0 };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, disable);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, disable);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, disable);
    glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
}

// 3. initialize OpenGL and load textures
void init()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    // glLoadIdentity();
    // light(light_position,light_color,0.6,0.2,1);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_NORMALIZE);
    texture = loadBMPTexture("assets/bricks.bmp");
    sun=loadBMPTexture("assets/2k_sun.bmp");
}

void createPlatformFromTextFile(const char* fileName, GLfloat cubeSize, GLuint texture) {
    ifstream file(fileName);
    string line;
    GLfloat x = 0, y = 0, z = 0;

    if (file.is_open()) {
        cout << "File is open" << endl;
        while (getline(file, line)) {
            for (int i = 0; i < line.length(); i++) {
                char c = line[i];
                if (c == 'o') {
                    new PlatformCube(x, y, z, cubeSize, texture);
                } else if (c == 'S') {
                    new PlatformCube(x, y, z, cubeSize, texture);
                    new Player(x, cubeSize+cubeSize/2, z, cubeSize, 0);
                }
                x += cubeSize;
            }
            z -= cubeSize;
            x = 0;
        }
        file.close();
    }
}

// 4. render the scene
void display()
{
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glBindTexture(GL_TEXTURE_2D, 0);
    camera(cameraRadius,angleX,angleY);
    light(light_position,light_color,0.6,0.3,1);
    glPushMatrix();
    material_white();
    // new Player(40,0,0,40,0);
    // new PlatformCube(0,0,0,40,texture);
    createPlatformFromTextFile("./src/a.txt", 40, texture);
    glPopMatrix();
    glutSwapBuffers();
    glFlush();
}

// 5. handle window resizing and set up the projection matrix
void reshape(int w, int h)
{
    int x = (w > h ? (w - min(w, h)) / 2 : 0);
    int y = (h > w ? (h - min(w, h)) / 2 : 0);
    glViewport(x, y, min(w, h), min(w, h));
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(windowWidth / -2, windowWidth / 2, windowHeight / -2, windowHeight / 2, z_near, z_far);
    glMatrixMode(GL_MODELVIEW);
}

// 6. update the scene at a fixed time interval
void timer(int t)
{
    glutPostRedisplay();
    glutTimerFunc(1000 / 60, timer, 0);
    // angle += 2; // stop auto rotation
}

// 7. handle keyboard input and move the camera
void keyboard(unsigned char ch, int, int)
{
    if(ch=='w')
        angleX=(angleX+1)%360;
    else if(ch=='s')
        angleX=(angleX-1)%360;

    if(ch=='d')
        angleY=(angleY+1)%360;
    else if(ch=='a')
        angleY=(angleY-1)%360;
}

// 8. initializes GLUT and starts the program loop
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowSize(1000, 1000);
    glutCreateWindow("Bloxorz");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, timer, 0);
    init();
    glutMainLoop();
    return 0;
}