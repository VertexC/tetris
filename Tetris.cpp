#include "Angel.h"

#include <iostream>
#include <stdlib.h>
using namespace std;

#define GRIDUNITSCALE 50
#define GRIDROWS 20
#define GRIDCOLS 10

// window size
int windowWidth = 600;
int windowHeight = 800;

// for vertex array and buffer
GLuint vaoIDs[3];
GLuint vboIDs[6];

// grid and unit in grid
vec2 gridOrigin = vec2(40, 20);
vec3 unitColors[6 * GRIDROWS * GRIDCOLS];
bool isUnitFill[GRIDROWS][GRIDCOLS];

// colors
vec3 red = vec3(1.0, 0.0, 0.0);
vec3 green = vec3(0.0, 1.0, 0.0);
vec3 blue = vec3(0.0, 0.0, 1.0);
vec3 white = vec3(1.0, 1.0, 1.0);
vec3 black = vec3(0.0, 0.0, 0.0);
vec3 grey = vec3(0.9, 0.9, 0.9);

// Tetris
// Record of all tetris and its rotations
vec2 tetrisDic[1][4][4] = {
    {{vec2(0, 0), vec2(-1, 0), vec2(1, 0), vec2(-1, -1)}, // "L"
     {vec2(0, 0), vec2(0, 1), vec2(0, -1), vec2(1, -1)},
     {vec2(0, 0), vec2(1, 0), vec2(1, 1), vec2(-1, 0)},
     {vec2(0, 0), vec2(0, 1), vec2(-1, 1), vec2(0, -1)}}};
// the teris moves on the screen, just one
vec2 tetris[4];
vec2 tetrisPosition = (10, 1);

/**
 * map the cordinate in grid to Window
 *
 * */
vec2 gridToWindow(vec2 input)
{
    vec2 output;
    output.x = -1.0 + (input.x * GRIDUNITSCALE + gridOrigin.x) * 1.0 / windowWidth;
    output.y = 1.0 - (input.y * GRIDUNITSCALE + gridOrigin.y) * 1.0 / windowHeight;
    return output;
}

/**
 * 
 *
 * */
void nextTeris()
{
    tetrisPosition.x = random() % GRIDCOLS;
    tetrisPosition.y = 1;

    int rotation = rand() % 4;
    //initialize the vertex position
    for (int i = 0; i < 4; i++)
    {
        tetris[i] = tetrisDic[0][rotation][i];
    }
    // update the color
    glBindBuffer(GL_ARRAY_BUFFER, vboIDs[4]);
    for (int i = 0; i < 4; i++)
    {
        int x = tetrisPosition.x + tetris[i].x;
        int y = tetrisPosition.y + tetris[i].y;
        vec2 leftup = gridToWindow(vec2(x, y));
        vec2 leftdown = gridToWindow(vec2(x, y + 1));
        vec2 rightup = gridToWindow(vec2(x + 1, y));
        vec2 rightdown = gridToWindow(vec2(x + 1, y + 1));
        // square with two triangles
        vec2 square[6] = {leftup,
                          leftdown,
                          rightdown,
                          leftup,
                          rightdown,
                          rightup};
        glBufferSubData(GL_ARRAY_BUFFER, i * 6 * sizeof(vec2), 6 * sizeof(vec2), square);
    }
    glBindVertexArray(0);

    // 4 square, 6 vertex each
    vec3 tetrisColor[24];
    for (int i = 0; i < 24; i++)
    {
        tetrisColor[i] = red;
    }
    glBindBuffer(GL_ARRAY_BUFFER, vboIDs[5]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(tetrisColor), tetrisColor);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void init()
{
    // to draw the line of the grid, 21 row, 11 col, 64 vertex, every color for one vertex
    vec2 gridPoints[64];
    vec3 gridColors[64];
    // set line of row
    for (int i = 0; i < GRIDROWS + 1; i++)
    {
        gridPoints[2 * i] = gridToWindow(vec2(0, i));
        gridPoints[2 * i + 1] = gridToWindow(vec2(GRIDCOLS, i));
    }

    // set line of col
    for (int i = 0; i < GRIDCOLS + 1; i++)
    {
        gridPoints[(GRIDROWS + 1) * 2 + 2 * i] = gridToWindow(vec2(i, 0));
        gridPoints[(GRIDROWS + 1) * 2 + 2 * i + 1] = gridToWindow(vec2(i, GRIDROWS));
    }
    // set color of line
    for (int i = 0; i < 64; i++)
    {
        gridColors[i] = grey;
    }

    // for every unit in the grid
    vec2 unitPoints[6 * GRIDROWS * GRIDCOLS]; // 20 * 10 square, 6 vertex per square
    // set the color of unit as black
    for (int i = 0; i < GRIDCOLS * GRIDROWS * 3; i++)
    {
        unitColors[i] = black;
    }
    // set the vertex of unit
    for (int i = 0; i < GRIDROWS; i++)
    {
        for (int j = 0; j < GRIDCOLS; j++)
        {
            vec2 leftup = gridToWindow(vec2(i, j));
            vec2 rightup = gridToWindow(vec2(i + 1, j));
            vec2 leftdown = gridToWindow(vec2(i, j + 1));
            vec2 rightdown = gridToWindow(vec2(i + 1, j + 1));

            unitPoints[6 * (10 * i + j)] = leftup;
            unitPoints[6 * (10 * i + j) + 1] = leftdown;
            unitPoints[6 * (10 * i + j) + 2] = rightdown;

            unitPoints[6 * (10 * i + j) + 3] = leftup;
            unitPoints[6 * (10 * i + j) + 4] = rightdown;
            unitPoints[6 * (10 * i + j) + 5] = rightup;
        }
    }
    // set isUnitFilled
    for (int i = 0; i < GRIDROWS; i++)
    {
        for (int j = 0; j < GRIDCOLS; j++)
        {
            isUnitFill[i][j] = false;
        }
    }
    // load in shader
    GLuint program = InitShader("vshader.glsl", "fshader.glsl");
    glUseProgram(program);

    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    GLuint vColor = glGetAttribLocation(program, "vColor");

    glGenVertexArrays(3, &vaoIDs[0]);

    // Unit
    glBindVertexArray(vaoIDs[0]);
    glGenBuffers(2, vboIDs);
    // Vertex Location
    glBindBuffer(GL_ARRAY_BUFFER, vboIDs[0]);
    glBufferData(GL_ARRAY_BUFFER, 64 * sizeof(vec2), gridPoints, GL_STATIC_DRAW);
    glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vPosition);
    // Vertex Color
    glBindBuffer(GL_ARRAY_BUFFER, vboIDs[1]);
    glBufferData(GL_ARRAY_BUFFER, 64 * sizeof(vec3), gridColors, GL_STATIC_DRAW);
    glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vColor);

    // Tetris
    glBindVertexArray(vaoIDs[2]);
    glGenBuffers(2, &vboIDs[4]);
    // Vertex Location
    glBindBuffer(GL_ARRAY_BUFFER, vboIDs[4]);
    glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(vec2), NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vPosition);
    // Vertex Color
    glBindBuffer(GL_ARRAY_BUFFER, vboIDs[5]);
    glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(vec3), NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vColor);

    glBindVertexArray(0);
    glClearColor(0, 0, 0, 0);

    nextTeris(); // game begin, show the teris
}

/**
 * 
 *
 * */
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(vaoIDs[2]);
    glDrawArrays(GL_TRIANGLES, 0, 24);

    glBindVertexArray(vaoIDs[0]);
    glDrawArrays(GL_LINES, 0, 64);

    glutSwapBuffers();
}
/**
 * 
 * 
 * */
void reshape(GLsizei w, GLsizei h)
{
    windowWidth = 800;
    windowHeight = 800;
    glViewport(0, 0, w, h);
}
/** 
 * 
 * 
 * */
void special(int key, int x, int y)
{
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
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(500, 500);
    glutCreateWindow("Tetris");
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