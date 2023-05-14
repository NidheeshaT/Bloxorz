#define MINIAUDIO_IMPLEMENTATION
#include "../headers/miniaudio.h"
#include "GLUT/glut.h"
#include <iostream>
#include <unordered_map>
#include <fstream>
#include <math.h>
#include "../headers/utilities.hpp"
#include "../headers/PlatformCube.hpp"
#include "../headers/Player.hpp"
#define PI 3.14159265
unsigned int prevTime = 0;
int time = 0;
ma_engine engine;
using namespace std;
float cameraRadius = 250;
int angleX = 60;
int angleY = 0;
int angle = 0;

float gameOverTimer = 0.0f;
const float GAME_OVER_DELAY = 2.0f;
float victoryTimer = 0.0f;
const float VICTORY_DELAY = 3.0f;
bool continueClicked = false;

int cameraLookX=0,cameraLookY=0,cameraLookZ=0;
float delta;

int windowWidth = 100;
int windowHeight = 100;
int z_near = 50;
int z_far = 800;
float light_position[] = {0, 400, -400, 1};
float light_color[] = {1, 0.7, 0.2};
int level=1;
GLuint texture, sun, stars,gold;
unordered_map<pair<int, int>, PlatformCube *, hash_pair> *Platform = new unordered_map<pair<int, int>, PlatformCube *, hash_pair>();
Player *P;
int targetX,targetZ;
float red=1.0,green=0,blue=0;

enum Scene
{
    MENU,
    PAUSE,
    GAME,
    GAMEOVER,
    VICTORY
};

Scene scene = MENU;
bool changedToGame = false;
void key_detect(int ch, int x, int y);

void camera(float cameraRadius, int angleX, int angleY)
{
    GLdouble cameraX, cameraY, cameraZ;
    cameraY = sin(PI * angleX / 180) * cameraRadius;
    cameraZ = cos(PI * angleX / 180) * cameraRadius;

    cameraX = sin(PI * angleY / 180) * cameraZ;
    cameraZ = cos(PI * angleY / 180) * cameraZ;
    if(P!=nullptr){
        if(abs(cameraLookX-P->x)<=abs((P->x-cameraLookX)*delta)){
            cameraLookX=P->x;
        }
        else{
            cameraLookX+=(P->x-cameraLookX)*delta;
        }
        // cameraLookY=P->y;
        if(abs(cameraLookZ-P->z)<=abs((P->z-cameraLookZ)*delta)){
            cameraLookZ=P->z;
        }
        else{
            cameraLookZ+=(P->z-cameraLookZ)*delta;
        }
    }
    gluLookAt(cameraX+cameraLookX, cameraY+cameraLookY, cameraZ+cameraLookZ, cameraLookX,cameraLookY,cameraLookZ, 0, cos(PI * angleX / 180), 0);

}
void material_white();
void material_emissive();
void drawString(const char *str, float x, float y, void *font);

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
void material(float red,float green,float blue)
{
    float color[] = {red, blue, green, 1};
    float disable[] = {0, 0, 0, 0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color);
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
    GLfloat mat_emission[] = {1, 1, 1, 1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, disable);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, disable);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, disable);
    glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
}
void menuProjection()
{
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
    stars = loadBMPTexture("assets/stars.bmp");
    gold = loadBMPTexture("assets/gold.bmp");
    menuProjection();
}

// 4. create platform from text file containing map [o = platform, S = player, T = target, - = empty space]
void createPlatformFromTextFile(const char *fileName, GLfloat cubeSize, GLuint texture)
{
    ifstream file(fileName);
    string line;
    GLfloat x = 0, y = 0, z = 0;
    PlatformCube *cube;
    int px,py,pz;
    if (file.is_open())
    {
        while (getline(file, line))
        {
            for (int i = 0; i < line.length(); i++)
            {
                char c = line[i];
                if (c == 'o')
                {
                    cube = new PlatformCube(x, y, z, cubeSize, texture);
                    Platform->insert({{x, z}, cube});
                }
                else if (c == 'S')
                {
                    cube=new PlatformCube(x, y, z, cubeSize, texture);
                    px=x;
                    py=cubeSize+cubeSize/2;
                    pz=z;
                    glutSpecialFunc(key_detect);
                    Platform->insert({{x, z}, cube});
                }
                else if(c=='T'){
                    targetX=x;
                    targetZ=z;
                    cube=new PlatformCube(x, y, z, cubeSize, gold);
                    Platform->insert({{x,z},cube});
                }
                x += cubeSize;
            }
            z -= cubeSize;
            x = 0;
            P=new Player(px,py , pz, cubeSize, 0,Platform);
            P->targetX=targetX;
            P->targetZ=targetZ;
        }
        file.close();
    }
}
void renderColor(){
    if(red==1 && green!=1){
        if(blue>0)
        {
            blue-=delta/2;
            if(blue<=0)
                blue=0;
        }
        else{
            green+=delta/2;
            if(green>=1)
                green=1;
        }
    }
    else if(green==1 && blue!=1){
        if(red>0)
        {
            red-=delta/2;
            if(red<=0)
                red=0;
        }
        else{
            blue+=delta/2;
            if(blue>=1)
                blue=1;
        }
    }
    else if(blue==1 && red!=1){
        if(green>0)
        {
            green-=delta/2;
            if(green<=0)
                green=0;
        }
        else{
            red+=delta/2;
            if(red>=1)
                red=1;
        }
    }
}

void renderGame(float delta)
{
    // renderColor();
    material(red,green,blue);
    P->render(delta);
    material_white();
    for(auto p=Platform->begin();p!=Platform->end();p++){
        if(p->second!=nullptr)
            p->second->render();
    }
    // Check if the player has fallen off
    if (P->inFall)
    {
        gameOverTimer += delta;
        if (gameOverTimer >= GAME_OVER_DELAY)
        {
            scene = GAMEOVER;
        }
    }
    else
    {
        gameOverTimer = 0;
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

void drawButton(const char *text, int x, int y, int buttonWidth, int buttonHeight, void *font)
{
    int textWidth = glutBitmapLength(font, (const unsigned char *)text);
    int buttonTextX = x + (buttonWidth - textWidth) / 2;
    int buttonTextY = y + (buttonHeight / 2) - 8;

    // Draw button
    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_QUADS);
    glVertex2i(x, y);
    glVertex2i(x, y + buttonHeight);
    glVertex2i(x + buttonWidth, y + buttonHeight);
    glVertex2i(x + buttonWidth, y);
    glEnd();

    // Draw button text
    glColor3f(1.0, 1.0, 1.0);
    drawString(text, buttonTextX, buttonTextY, font);
}

void menuScreen()
{
    glClearColor(0, 0, 0, 0);

    glPushMatrix();

    // Draw text
    const char *text = "Welcome to Bloxorz!";
    int textWidth = glutBitmapLength(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char *)text);
    int x = (glutGet(GLUT_WINDOW_WIDTH) - textWidth) / 2;
    int y = glutGet(GLUT_WINDOW_HEIGHT) / 2 + 50;
    drawString(text, x, y, GLUT_BITMAP_TIMES_ROMAN_24);

    // Draw button
    int buttonWidth = 200;
    int buttonHeight = 50;
    int buttonX = (glutGet(GLUT_WINDOW_WIDTH) - buttonWidth) / 2;
    int buttonY = y - 80;
    drawButton("Play >>", buttonX, buttonY, buttonWidth, buttonHeight, GLUT_BITMAP_TIMES_ROMAN_24);

    // Draw instructions
    const char *instructions1 = "Instructions:";
    const char *instructions2 = "WASD to move camera";
    const char *instructions3 = "Arrows to move block";
    int instructionsX = (glutGet(GLUT_WINDOW_WIDTH) - glutBitmapLength(GLUT_BITMAP_HELVETICA_12, (const unsigned char *)instructions1)) / 2.3;
    int instructionsY = buttonY - 50;
    drawString(instructions1, instructionsX, instructionsY, GLUT_BITMAP_HELVETICA_12);
    drawString(instructions2, instructionsX, instructionsY - 30, GLUT_BITMAP_HELVETICA_12);
    drawString(instructions3, instructionsX, instructionsY - 60, GLUT_BITMAP_HELVETICA_12);

    glPopMatrix();
}

void pauseScreen()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT));
    glMatrixMode(GL_MODELVIEW);

    // Disable lighting so text is not affected by lighting
    glDisable(GL_LIGHTING);

    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();

    // Draw button
    int y = glutGet(GLUT_WINDOW_HEIGHT) / 2 + 50;
    int buttonWidth = 200;
    int buttonHeight = 50;
    int buttonX = (glutGet(GLUT_WINDOW_WIDTH) - buttonWidth) / 2;
    int buttonY = y - 80;
    drawButton("Resume", buttonX, buttonY, buttonWidth, buttonHeight, GLUT_BITMAP_TIMES_ROMAN_24);

    glColor3f(1.0, 1.0, 1.0);
    const char *text = "PAUSED";
    int X = (glutGet(GLUT_WINDOW_WIDTH) - glutBitmapLength(GLUT_BITMAP_HELVETICA_12, (const unsigned char *)text)) / 2.3;
    int Y = buttonY - 50;
    void *font = GLUT_BITMAP_TIMES_ROMAN_24;
    drawString(text, X, Y, font);

    glPopMatrix();

    // Re-enable lighting and depth test
    glEnable(GL_LIGHTING);
}

void starryBackground(){
    glPushMatrix();
        material_emissive_white();
        glBindTexture(GL_TEXTURE_2D, stars);
        GLUquadricObj *sphere = gluNewQuadric();
        gluQuadricTexture(sphere, GL_TRUE);
        gluQuadricNormals(sphere, GLU_SMOOTH);
        glTranslatef(cameraLookX,cameraLookY,cameraLookZ);
        gluSphere(sphere, 550.0, 100, 100);
        gluDeleteQuadric(sphere);
    glPopMatrix();
}

void menuBar(){
    glPushMatrix();
        glTranslatef(cameraLookX,cameraLookY,cameraLookZ);
        glRotatef(angleY,0,1,0);
        glRotatef(-angleX,1,0,0);
        glColor3f(0,1,0.6);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_LIGHTING);
        string str="Number of moves: "+to_string(P->moves)+
        "                                                          "+
        "                                 "
        +"Level: "+to_string(level);
        drawString(str.c_str(),-200,200,GLUT_BITMAP_HELVETICA_18);
        drawString("Press P to Pause",-20,-200,GLUT_BITMAP_HELVETICA_18);
        glEnable(GL_LIGHTING);
        glEnable(GL_DEPTH_TEST);
    glPopMatrix();
}

void gameScreen(float delta)
{
    glBindTexture(GL_TEXTURE_2D, 0);
    camera(cameraRadius, angleX, angleY);
    light(light_position, light_color, 0.6, 0.4, 1);
    starryBackground();
    glPushMatrix();
    material_white();
    if (changedToGame)
    {
        changedToGame = false;
        Platform->clear();
        delete P;
        if(level==4)
            level=1;
        string path = "./maps/l" + to_string(level) + ".txt";
        createPlatformFromTextFile(path.c_str(), 40, texture);
    }
    else
    {
        renderGame(delta);
    }
    menuBar();
    glPopMatrix();
}

void endScreen()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT));
    glMatrixMode(GL_MODELVIEW);

    // Disable lighting so text is not affected by lighting
    glDisable(GL_LIGHTING);

    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();

    // Draw button
    int y = glutGet(GLUT_WINDOW_HEIGHT) / 2 + 50;
    int buttonWidth = 200;
    int buttonHeight = 50;
    int buttonX = (glutGet(GLUT_WINDOW_WIDTH) - buttonWidth) / 2;
    int buttonY = y - 80;
    drawButton("Restart", buttonX, buttonY, buttonWidth, buttonHeight, GLUT_BITMAP_TIMES_ROMAN_24);

    glColor3f(1.0, 1.0, 1.0);
    const char *text = "Game Over!";
    int X = (glutGet(GLUT_WINDOW_WIDTH) - glutBitmapLength(GLUT_BITMAP_HELVETICA_12, (const unsigned char *)text)) / 2.3;
    int Y = buttonY - 50;
    void *font = GLUT_BITMAP_TIMES_ROMAN_24;
    drawString(text, X, Y, font);

    glPopMatrix();

    // Re-enable lighting and depth test
    glEnable(GL_LIGHTING);
}

void victoryScreen()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT));
    glMatrixMode(GL_MODELVIEW);

    // Disable lighting so text is not affected by lighting
    glDisable(GL_LIGHTING);

    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();

    // Draw button
    int y = glutGet(GLUT_WINDOW_HEIGHT) / 2 + 50;
    int buttonWidth = 200;
    int buttonHeight = 50;
    int buttonX = (glutGet(GLUT_WINDOW_WIDTH) - buttonWidth) / 2;
    int buttonY = y - 80;
    drawButton("Continue", buttonX, buttonY, buttonWidth, buttonHeight, GLUT_BITMAP_TIMES_ROMAN_24);

    glColor3f(1.0, 1.0, 1.0);
    const char *text = "You won!";
    int X = (glutGet(GLUT_WINDOW_WIDTH) - glutBitmapLength(GLUT_BITMAP_HELVETICA_12, (const unsigned char *)text)) / 2.3;
    int Y = buttonY - 50;
    void *font = GLUT_BITMAP_TIMES_ROMAN_24;
    drawString(text, X, Y, font);

    glPopMatrix();

    // Re-enable lighting and depth test
    glEnable(GL_LIGHTING);
}

// 5. render the scene
void display()
{
    unsigned int currTime = glutGet(GLUT_ELAPSED_TIME);
    delta = (float)(currTime - prevTime) / 1000.0f;
    prevTime = currTime;

    cout<<ma_engine_get_time(&engine)<<endl;

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    if(continueClicked){
        scene = GAME;
        continueClicked = false;
    } else if(P!=nullptr && P->inTarget){
        victoryTimer += delta;
        if (victoryTimer >= VICTORY_DELAY)
        {
            scene = VICTORY;
            victoryTimer = 0;
        }
    }

    if (scene == MENU){
        menuScreen();
    }
    else if (scene == PAUSE){
        pauseScreen();
    }
    else if (scene == GAME){
        gameScreen(delta);
    }
    else if (scene == GAMEOVER){
        endScreen();
    } 
    else if (scene == VICTORY){
        victoryScreen();
    }

    glutSwapBuffers();
    glFlush();
}

void gameProjection()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(windowWidth / -2, windowWidth / 2, windowHeight / -2, windowHeight / 2, z_near, z_far);
    glMatrixMode(GL_MODELVIEW);
}

void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && scene != GAME)
    {
        int buttonWidth = 200;
        int buttonHeight = 50;
        int buttonX = (glutGet(GLUT_WINDOW_WIDTH) - buttonWidth) / 2;
        int buttonY = glutGet(GLUT_WINDOW_HEIGHT) / 2 + 50 - 80;

        if (scene == PAUSE && x >= buttonX && x <= buttonX + buttonWidth &&
            y >= buttonY && y <= buttonY + buttonHeight)
        {
            scene = GAME;
            changedToGame = false;
            gameProjection();
        }
        else if (scene == GAMEOVER || scene == MENU && x >= buttonX && x <= buttonX + buttonWidth &&
                                          y >= buttonY && y <= buttonY + buttonHeight)
        {
            scene = GAME;
            changedToGame = true;
            gameProjection();
        } else if(scene == VICTORY && x >= buttonX && x <= buttonX + buttonWidth &&
                                          y >= buttonY && y <= buttonY + buttonHeight){
            continueClicked = true;
            level++;
            scene = GAME;
            changedToGame = true;
            gameProjection();
        }
    }
}

// 6. handle window resizing and set up the projection matrix
void reshape(int w, int h)
{
    int x = (w > h ? (w - min(w, h)) / 2 : 0);
    int y = (h > w ? (h - min(w, h)) / 2 : 0);
    glViewport(x, y, min(w, h), min(w, h));
    if (scene == MENU)
    {
        menuProjection();
    }
    else
    {
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

    if (ch == 'p' || ch == 'P')
    {
        if (scene == GAME)
            scene = PAUSE;
    }
}

void key_detect(int ch, int x, int y)
{
    P->key_detect(ch, x, y);
}

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
    if (pDecoder == NULL) {
        return;
    }

    /* Reading PCM frames will loop based on what we specified when called ma_data_source_set_looping(). */
    ma_data_source_read_pcm_frames(pDecoder, pOutput, frameCount, NULL);

    (void)pInput;
}


// 9. initializes GLUT and starts the program loop
int main(int argc, char **argv)
{
    ma_result result;
    ma_decoder decoder;
    ma_device_config deviceConfig;
    ma_device device;

    result = ma_decoder_init_file("./assets/gameplay.mp3", NULL, &decoder);
    if (result != MA_SUCCESS) {
        return -2;
    }

    ma_data_source_set_looping(&decoder, MA_TRUE);

    deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format   = decoder.outputFormat;
    deviceConfig.playback.channels = decoder.outputChannels;
    deviceConfig.sampleRate        = decoder.outputSampleRate;
    deviceConfig.dataCallback      = data_callback;
    deviceConfig.pUserData         = &decoder;

    if (ma_device_init(NULL, &deviceConfig, &device) != MA_SUCCESS) {
        printf("Failed to open playback device.\n");
        ma_decoder_uninit(&decoder);
        return -3;
    }

    if (ma_device_start(&device) != MA_SUCCESS) {
        printf("Failed to start playback device.\n");
        ma_device_uninit(&device);
        ma_decoder_uninit(&decoder);
        return -4;
    }
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

    ma_device_uninit(&device);
    ma_decoder_uninit(&decoder);
    return 0;
}