import os
import sys
if len(sys.argv)==2 and sys.argv[1]=='win':
    # Windows
    if len(sys.argv)==3 and sys.argv[2]=='static':
        os.system('g++ -I./include -L./lib src/*.cpp -D FREEGLUT_STATIC -lfreeglut_static -lglu32 -lopengl32  -lwinmm -lgdi32 -o main && main')
    else:
        os.system('g++ -I./include -L./lib src/*.cpp -lfreeglut -lglu32 -lopengl32 -o main && main')
else:
    # Mac
    os.system('g++ -std=c++20 -framework OpenGL -framework GLUT src/*.cpp && ./a.out')