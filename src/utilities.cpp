#include "../headers/utilities.hpp"

/* Cube - Platform */
// 1. Namespace for cube vertices, normals, texture coords and faces.
namespace cube
{
    GLfloat vertices[8][3] = {
        {-1.0, -1.0, 1.0},
        {1.0, -1.0, 1.0},
        {1.0, 1.0, 1.0},
        {-1.0, 1.0, 1.0},
        {-1.0, -1.0, -1.0},
        {1.0, -1.0, -1.0},
        {1.0, 1.0, -1.0},
        {-1.0, 1.0, -1.0},
    };
    GLfloat normals[6][3] = {
        {0.0, 0.0, 1.0},  // front
        {0.0, 0.0, -1.0}, // back
        {1.0, 0.0, 0.0},  // right
        {-1.0, 0.0, 0.0}, // left
        {0.0, 1.0, 0.0},  // top
        {0.0, -1.0, 0.0}, // bottom
    };
    GLfloat texcoords[4][2] = {
        {0.0, 0.0},
        {1.0, 0.0},
        {1.0, 1.0},
        {0.0, 1.0},
    };
    GLfloat faces[6][4] = {
        {0, 1, 2, 3}, // front
        {5, 4, 7, 6}, // back
        {1, 5, 6, 2}, // right
        {4, 0, 3, 7}, // left
        {3, 2, 6, 7}, // top
        {4, 5, 1, 0}, // bottom
    };

}

// 2. Draws a cube at the given position and size.
void drawCube()
{
    glBegin(GL_QUADS);

    for (int i = 0; i < 6; i++)
    {
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


/* Cuboid - Block */
// 1. Namespace for cuboid vertices, normals, texture coordinates and faces.
namespace cuboid
{
    GLfloat vertices[8][3] = {
        {-1.0, -2.0, 1.0}, 
        {1.0, -2.0, 1.0},
        {1.0, 2.0, 1.0},
        {-1.0, 2.0, 1.0},
        {-1.0, -2.0, -1.0},
        {1.0, -2.0, -1.0},
        {1.0, 2.0, -1.0},
        {-1.0, 2.0, -1.0},
    };
    GLfloat normals[6][3] = {
        {0.0, 0.0, 1.0},  // front
        {0.0, 0.0, -1.0}, // back
        {1.0, 0.0, 0.0},  // right
        {-1.0, 0.0, 0.0}, // left
        {0.0, 1.0, 0.0},  // top
        {0.0, -1.0, 0.0}, // bottom
    };
    GLfloat texcoords[4][2] = {
        {0.0, 0.0},
        {1.0, 0.0},
        {1.0, 1.0},
        {0.0, 1.0},
    };
    GLfloat faces[6][4] = {
        {0, 1, 2, 3}, // front
        {5, 4, 7, 6}, // back
        {1, 5, 6, 2}, // right
        {4, 0, 3, 7}, // left
        {3, 2, 6, 7}, // top
        {4, 5, 1, 0}, // bottom
    };
}

// 2. Draws a cuboid at the given position and size.
void drawCuboid()
{
    glBegin(GL_QUADS);
    for (int i = 0; i < 6; i++)
    {
        glNormal3fv(&cuboid::normals[i][0]);
        glTexCoord2fv(&cuboid::texcoords[0][0]);
        glVertex3fv(&cuboid::vertices[(int)cuboid::faces[i][0]][0]);
        glTexCoord2fv(&cuboid::texcoords[1][0]);
        glVertex3fv(&cuboid::vertices[(int)cuboid::faces[i][1]][0]);
        glTexCoord2fv(&cuboid::texcoords[2][0]);
        glVertex3fv(&cuboid::vertices[(int)cuboid::faces[i][2]][0]);
        glTexCoord2fv(&cuboid::texcoords[3][0]);
        glVertex3fv(&cuboid::vertices[(int)cuboid::faces[i][3]][0]);
    }
    glEnd();
}


// 1. loads the BMP image file as a texture for OpenGL
GLuint loadBMPTexture(const char *filename)
{
    GLuint texture;

    FILE *file;
    unsigned char header[54];
    unsigned int dataPos;
    unsigned int imageSize;
    unsigned char *data;

    // Open the file
    file = fopen(filename, "rb");
    if (!file)
    {
        printf("Error: could not open BMP file %s\n", filename);
        return 0;
    }

    // Read the header
    if (fread(header, 1, 54, file) != 54)
    {
        printf("Error: invalid BMP file %s\n", filename);
        fclose(file);
        return 0;
    }

    // Check the header
    if (header[0] != 'B' || header[1] != 'M')
    {
        printf("Error: invalid BMP file %s\n", filename);
        fclose(file);
        return 0;
    }

    // Read the data position and image size
    dataPos = *(int *)&(header[0x0A]);
    imageSize = *(int *)&(header[0x22]);
    if (imageSize == 0)
        imageSize = *(int *)&(header[0x02]) - dataPos;

    int width=*(int*)&header[18];
    int height=*(int*)&header[22];
    // Allocate memory for the image data
    data = (unsigned char *)malloc(imageSize);

    // Read the image data
    fseek(file, dataPos, SEEK_SET);
    if (fread(data, 1, imageSize, file) != imageSize)
    {
        printf("Error: invalid BMP file %s\n", filename);
        fclose(file);
        free(data);
        return 0;
    }

    // Close the file
    fclose(file);

    // Create OpenGL texture
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,
              GL_MODULATE);
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, header[18], header[22], 0, 0x80E0, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                    GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                    GL_CLAMP);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width,height, 0x80E0, GL_UNSIGNED_BYTE, data);

    glBindTexture(GL_TEXTURE_2D, 0);
    // Free the image data
    free(data);

    return texture;
}

// 2. frees the specified texture
void freetexture(GLuint texture)
{
    glDeleteTextures(1, &texture);
}