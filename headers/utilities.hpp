#include<GLUT/glut.h>
#include<iostream>

namespace cube{
            GLfloat vertices[8][3]={
        {-1.0,-1.0,1.0},
        {1.0,-1.0,1.0},
        {1.0,1.0,1.0},
        {-1.0,1.0,1.0},
        {-1.0,-1.0,-1.0},
        {1.0,-1.0,-1.0},
        {1.0,1.0,-1.0},
        {-1.0,1.0,-1.0},
    };
    GLfloat normals[6][3]={
        {0.0,0.0,1.0},//front
        {0.0,0.0,-1.0},//back
        {1.0,0.0,0.0},//right
        {-1.0,0.0,0.0},//left
        {0.0,1.0,0.0},//top
        {0.0,-1.0,0.0},//bottom
    };
    GLfloat texcoords[4][2]={
        {0.0,0.0},
        {1.0,0.0},
        {1.0,1.0},
        {0.0,1.0},
    };
    GLfloat faces[6][4]={
        {0,1,2,3},//front
        {5,4,7,6},//back
        {1,5,6,2},//right
        {4,0,3,7},//left
        {3,2,6,7},//top
        {4,5,1,0},//bottom
    };

}

GLuint loadtextures(const char *filename, float width, float height)
{
    GLuint texture;

    unsigned char *data;
    FILE *file;

    file = fopen(filename, "rb");
    if (file == NULL)
        return 0;

    data = (unsigned char *)malloc(width * height * 3);
    fread(data, width * height * 3, 1, file);

    fclose(file);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,
              GL_MODULATE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                    GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                    GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                    GL_CLAMP);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height,
                      GL_RGB, GL_UNSIGNED_BYTE, data);
    // glBindTexture(GL_TEXTURE_2D, 0);
    free(data);
    data = NULL;

    return texture;
}


void freetexture (GLuint texture) {
    glDeleteTextures( 1, &texture );
}

void drawCube(GLfloat x,GLfloat y,GLfloat z,GLfloat size=1.0f){

    glTranslatef(x,y,z);
    glScalef(size,size,size);
    glBegin(GL_QUADS);
    for(int i=0;i<6;i++){
        glNormal3fv(&cube::normals[i][0]);
        glTexCoord2fv(&cube::texcoords[0][0]);
        glVertex3fv(&cube::vertices[(int)cube::faces[i][0]][0]);
        glTexCoord2fv(&cube::texcoords[1][0]);
        glVertex3fv(&cube::vertices[(int)cube::faces[i][1]][0]);
        glTexCoord2fv(&cube::texcoords[2][0]);
        glVertex3fv(&cube::vertices[(int)cube::faces[i][2]][0]);
        glTexCoord2fv(&cube::texcoords[3][0]);
        glVertex3fv(&cube::vertices[(int)cube::faces[i][3]][0]);
    }
    glEnd();
}
