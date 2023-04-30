#ifndef UTILITIES_H

#define UTILITIES_H

#include <GLUT/glut.h>
#include <iostream>

void drawCube();
void drawCuboid();
GLuint loadBMPTexture(const char *filename);
struct hash_pair {
    template<class T1, class T2>
    size_t operator()(const std::pair<T1, T2>& p) const {
        auto hash1 = std::hash<T1>{}(p.first);
        auto hash2 = std::hash<T2>{}(p.second);
        return hash1 ^ hash2;
    }
};
#endif