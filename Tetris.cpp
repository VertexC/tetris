#include "Angel.h"

#include <iostream>

using namespace std;

// window size
windowWidth = 800;
windowHeight = 800

/**
 * 
 *
 * */
void 
/**
 * 
 * 
 * */  
void reshape(GLsizei w, GLsizei h){
    windowWidth = 800;
    windowHeight = 800;
    glViewport(0, 0, w, h);
}
/** 
 * 
 * 
 * */  
void special(int key, int x, int y){

}
/**
 * 
 *
 * */
void keyboard(unsigned char key, int x, int y)
{

}

/*
 *  
 * 
 * */
void idle(void)
{
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitContextVersion(3, 3);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(xsize, ysize);
    glutInitWindowPosition(500, 500);
    glutCreateSubWindow("Tetris");
    glewExperimental = GL_TRUE;
    glewInit();
    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(special);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    glutMainLoop();

    return 0;
}