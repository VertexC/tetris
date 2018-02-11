#include "Angel.h"

#include <iostream>

using namespace std;

int main(int argc, char ** argv){
    glutInit(&argc, argv);
    glutInitContextVersion(3,3);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(xsize, ysize);
    glutInitWindowPosition(500,500);
    glutCreateSubWindow("Tetris");
    glewExperimental = GL_TRUE;
    glewInit();
    init();

    return 0;
}