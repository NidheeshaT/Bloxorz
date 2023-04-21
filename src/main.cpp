#include "GLUT/glut.h"
#include <iostream>
#include<unordered_map>
#include <fstream>
#include <math.h>
#include "../headers/utilities.hpp"
#include "../headers/PlatformCube.hpp"
#include "../headers/Player.hpp"
#define PI 3.14159265
unsigned int prevTime = 0;


using namespace std;
float cameraRadius = 300;
int angleX = 0;
int angleY = 0;
int angle = 0;

int windowWidth = 100;
int windowHeight = 100;
int z_near = 50;
int z_far = 800;
float light_position[] = {0, 600, -600, 1};
float light_color[] = {1, 0.7, 0.2};
struct hash_pair {
    template<class T1, class T2>
    size_t operator()(const std::pair<T1, T2>& p) const {
        auto hash1 = std::hash<T1>{}(p.first);
        auto hash2 = std::hash<T2>{}(p.second);
        return hash1 ^ hash2;
    }
};
GLuint texture, sun;
unordered_map<pair<int,int>,PlatformCube*,hash_pair> Platform;
Player *P;

enum Scene
{
    MENU,
    GAME,
    GAMEOVER
};

Scene scene = MENU;
bool changedToGame=false;
void key_detect(int ch,int x,int y);

void camera(float cameraRadius, int angleX, int angleY)
{
    GLdouble cameraX, cameraY, cameraZ;
    cameraY = sin(PI * angleX / 180) * cameraRadius;
    cameraZ = cos(PI * angleX / 180) * cameraRadius;

    cameraX = sin(PI * angleY / 180) * cameraZ;
    cameraZ = cos(PI * angleY / 180) * cameraZ;
    gluLookAt(cameraX, cameraY, cameraZ, 0, 0, 0, 0, cos(PI * angleX / 180), 0);
}
void material_white();
void material_emissive();

// 1. set up the lighting in the scene
void light(float position[4], float color[3], float diffuse_strength, float ambient_strength, float specular_strength)
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
    GLfloat mat_emission[] = {0.6f, 0.4784f, 0.0f, 1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, disable);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, disable);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, disable);
    glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
}
void material_emissive_white()
{
    float disable[] = {0, 0, 0, 0};
    GLfloat mat_emission[] = {1, 0.8, 2, 1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, disable);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, disable);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, disable);
    glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
}
void menuProjection(){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT));
    glMatrixMode(GL_MODELVIEW);
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
    sun = loadBMPTexture("assets/2k_sun.bmp");
    menuProjection();
}

// 4. create platform from text file containing map [o = platform, S = player, T = target, - = empty space]
void createPlatformFromTextFile(const char *fileName, GLfloat cubeSize, GLuint texture)
{
    ifstream file(fileName);
    string line;
    GLfloat x = 0, y = 0, z = 0;
    PlatformCube *cube;
    if (file.is_open())
    {
        while (getline(file, line))
        {
            for (int i = 0; i < line.length(); i++)
            {
                char c = line[i];
                if (c == 'o')
                {
                    cube=new PlatformCube(x, y, z, cubeSize, texture);
                    Platform.insert({{x,z},cube});
                }
                else if (c == 'S')
                {
                    cube=new PlatformCube(x, y, z, cubeSize, texture);
                    P=new Player(x, cubeSize+cubeSize/2, z, cubeSize, 0);
                    glutSpecialFunc(key_detect);
                    Platform.insert({{x,z},cube});
                }
                x += cubeSize;
            }
            z -= cubeSize;
            x = 0;
        }
        file.close();
    }
}
void renderGame(float delta){
    P->render(delta);
    for(auto p=Platform.begin();p!=Platform.end();p++){
        p->second->render();
    }
}
void drawString(const char *str, float x, float y, void *font)
{
    glRasterPos2f(x, y);
    for (const char *c = str; *c != '\0'; c++)
    {
        glutBitmapCharacter(font, *c);
    }
}

void menuScreen()
{
    glClearColor(0, 0, 0, 0);
    // Set up text parameters
    // glColor3f(1.0, 1.0, 1.0);

    glPushMatrix();

    // Draw text
    const char *text = "Hello, Play Bloxorz!";
    int textWidth = glutBitmapLength(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char *)text);
    int x = (glutGet(GLUT_WINDOW_WIDTH) - textWidth) / 2;
    int y = glutGet(GLUT_WINDOW_HEIGHT) / 2 + 50;
    drawString(text, x, y, GLUT_BITMAP_TIMES_ROMAN_24);

    // Draw button
    int buttonWidth = 200;
    int buttonHeight = 50;
    int buttonX = (glutGet(GLUT_WINDOW_WIDTH) - buttonWidth) / 2;
    int buttonY = y - 80;
    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_QUADS);
    glVertex2i(buttonX, buttonY);
    glVertex2i(buttonX, buttonY + buttonHeight);
    glVertex2i(buttonX + buttonWidth, buttonY + buttonHeight);
    glVertex2i(buttonX + buttonWidth, buttonY);
    glEnd();

    // Draw button text
    glColor3f(1.0, 1.0, 1.0);
    const char *buttonText = "Play >>";
    int buttonTextWidth = glutBitmapLength(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char *)buttonText);
    int buttonTextX = buttonX + (buttonWidth - buttonTextWidth) / 2;
    int buttonTextY = buttonY + (buttonHeight / 2) - 8;
    drawString(buttonText, buttonTextX, buttonTextY, GLUT_BITMAP_TIMES_ROMAN_24);

    // Draw click anywhere text
    const char *clickText = "Click anywhere to start game";
    int clickTextWidth = glutBitmapLength(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char *)clickText);
    int clickTextX = (glutGet(GLUT_WINDOW_WIDTH) - clickTextWidth) / 2;
    int clickTextY = buttonY - 30;
    drawString(clickText, clickTextX, clickTextY, GLUT_BITMAP_TIMES_ROMAN_24);

    glPopMatrix();
    // glMatrixMode(GL_PROJECTION);
    // glPopMatrix();
}

void gameScreen(float delta)
{
    glBindTexture(GL_TEXTURE_2D, 0);
    camera(cameraRadius, angleX, angleY);
    light(light_position, light_color, 0.6, 0.3, 1);
    glPushMatrix();
    material_white();
    if(changedToGame){
        changedToGame=false;
        createPlatformFromTextFile("./maps/l1.txt", 40, texture);
    }
    else{
        renderGame(delta);
    }
    glPopMatrix();
}

void endScreen()
{
}

// 5. render the scene
void display()
{
    unsigned int currTime = glutGet(GLUT_ELAPSED_TIME);
    float delta = (float)(currTime - prevTime) / 1000.0f;
    prevTime = currTime;

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    if (scene == MENU)
    {
        menuScreen();
    }
    else if (scene == GAME)
    {
        gameScreen(delta);
    }
    else
    {
        endScreen();
    }
    glutSwapBuffers();
    glFlush();
}

void gameProjection(){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(windowWidth / -2, windowWidth / 2, windowHeight / -2, windowHeight / 2, z_near, z_far);
    glMatrixMode(GL_MODELVIEW);
}
void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && scene!=GAME)
    {
        scene = GAME;
        changedToGame=true;
        gameProjection();
        // gameScreen(0);
    }
}

// 6. handle window resizing and set up the projection matrix
void reshape(int w, int h)
{
    int x = (w > h ? (w - min(w, h)) / 2 : 0);
    int y = (h > w ? (h - min(w, h)) / 2 : 0);
    glViewport(x, y, min(w, h), min(w, h));
    if(scene == MENU){
        menuProjection();
    }
    else{
        gameProjection();
    }
}

// 7. update the scene at a fixed time interval
void timer(int t)
{
    glutPostRedisplay();
    glutTimerFunc(1000 / 60, timer, 0);
    // angle += 2; // stop auto rotation
}

// 8. handle keyboard input and move the camera
void keyboard(unsigned char ch, int x, int y)
{
    if (ch == 'w')
        angleX = (angleX + 10) % 360;
    else if (ch == 's')
        angleX = (angleX - 10) % 360;

    if (ch == 'd')
        angleY = (angleY + 10) % 360;
    else if (ch == 'a')
        angleY = (angleY - 10) % 360;
}

void key_detect(int ch,int x,int y){
    P->key_detect(ch,x,y);
}

// 9. initializes GLUT and starts the program loop
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowSize(1000, 1000);
    glutCreateWindow("Bloxorz");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutTimerFunc(0, timer, 0);
    init();
    glutMainLoop();
    return 0;
}