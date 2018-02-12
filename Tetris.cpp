#include "Angel.h"

#include <iostream>
#include <stdlib.h>
using namespace std;

#define GRIDUNITSCALE 50
#define GRIDROWS 20
#define GRIDCOLS 20

// window size
int windowWidth = GRIDCOLS * GRIDUNITSCALE;
int windowHeight = GRIDROWS * GRIDUNITSCALE;

// for vertex array and buffer
GLuint vaoIDs[3];
GLuint vboIDs[6];

// grid and unit in grid
vec2 gridOrigin = vec2(0, 0);
vec3 unitColors[6 * GRIDROWS * GRIDCOLS];
bool isUnitFill[GRIDCOLS][GRIDROWS];

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
vec2 tetrisOriginPosition = (4, 1);
int rotation = 0;
int type = 0; 
float speed = 1000;

/**
 * map the cordinate in grid to Window
 *
 * */
vec2 gridToWindow(vec2 input)
{
    vec2 output = vec2(0, 0);
    // cout << input << endl;
    output.x = -1.0 + (input.x * GRIDUNITSCALE * 1.0 + gridOrigin.x * 1.0) * 1.0 / windowWidth;
    output.y = 1.0 - (input.y * GRIDUNITSCALE * 1.0 + gridOrigin.y * 1.0) * 1.0 / windowHeight;
    return output;
}

void updateTerisPosition()
{
    // update the position
    glBindBuffer(GL_ARRAY_BUFFER, vboIDs[4]);
    cout << tetris[0] << tetris[1] << tetris[2] << tetris[3] << endl;
    for (int i = 0; i < 4; i++)
    {
        int x = tetrisOriginPosition.x + tetris[i].x;
        int y = tetrisOriginPosition.y + tetris[i].y;
        vec2 leftup = gridToWindow(vec2(x, y));
        vec2 leftdown = gridToWindow(vec2(x, y + 1.0));
        vec2 rightup = gridToWindow(vec2(x + 1.0, y));
        vec2 rightdown = gridToWindow(vec2(x + 1.0, y + 1.0));
        cout << leftup << " " << leftdown << " " << rightup << " " << rightdown << endl;
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
    // glutPostRedisplay();
    cout << "Tetris updated" << endl;
}
/**
 * 
 *
 * */
void nextTeris()
{
    tetrisOriginPosition.x = 2 + random() % (GRIDCOLS - 1 - 2) ;
    tetrisOriginPosition.y = 1;
    rotation = random() % 4;
    type = 0;
    //initialize the vertex position
    for (int i = 0; i < 4; i++)
    {
        tetris[i] = tetrisDic[type][rotation][i];
    }
    updateTerisPosition();
    // set color, 4 square, 6 vertex each
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
    vec2 gridPoints[(GRIDCOLS + GRIDROWS + 2) * 2];
    vec3 gridColors[(GRIDCOLS + GRIDROWS + 2) * 2];
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
    for (int i = 0; i < (GRIDCOLS + GRIDROWS + 2) * 2; i++)
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
    for (int i = 0; i < GRIDCOLS; i++)
    {
        for (int j = 0; j < GRIDROWS; j++)
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
    glBufferData(GL_ARRAY_BUFFER, (GRIDCOLS + GRIDROWS + 2) * 2 * sizeof(vec2), gridPoints, GL_STATIC_DRAW);
    glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vPosition);
    // Vertex Color
    glBindBuffer(GL_ARRAY_BUFFER, vboIDs[1]);
    glBufferData(GL_ARRAY_BUFFER, (GRIDCOLS + GRIDROWS + 2) * 2 * sizeof(vec3), gridColors, GL_STATIC_DRAW);
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
    glDrawArrays(GL_LINES, 0, (GRIDCOLS + GRIDROWS + 2) * 2);

    glutSwapBuffers();
}
/**
 * 
 * 
 * */
void reshape(GLsizei w, GLsizei h)
{
    windowWidth = w;
    windowHeight = h;
    glViewport(0, 0, w, h);
}

/**
 * check if the movement is valid within the grid
 * 
 * */
bool isvalid(vec2 position)
{
    // check out of grid
    if (position.x < 0 || position.x >= GRIDCOLS || position.y < 0 || position.y >= GRIDROWS)
    {
        return false;
    }
    // check unit valid
    if (isUnitFill[int(position.x)][int(position.y)])
    {
        return false;
    }
    return true;
}

/**
 * move the tetris
 * */
void moveTetris(vec2 movement)
{
    // set the new position in the grid
    vec2 newOriginPosition = tetrisOriginPosition + movement;
    bool valid = true;
    for (int i = 0; i < 4; i++)
    {
        // map every unit of tetris to the grid, check if it is valid movement
        if (!isvalid(newOriginPosition + tetris[i]))
        {
            valid = false;
        }
    }
    if (valid)
    {
        // update the tetris position in the grid
        tetrisOriginPosition += movement;
        updateTerisPosition();
    }
}
/**
 * roate the tetris
 * */
void rotate(){
    // update the direction
    rotation = (rotation + 1) % 4;
    for (int i = 0; i < 4; i++){
        tetris[i] = tetrisDic[type][rotation][i];
    }
    updateTerisPosition();
}

/** 
 * use arrow key to ratate and move the tetris
 * */
void special(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_UP:
        // rotate the tetris
        rotate();
        break;
    case GLUT_KEY_DOWN:
        // move down the tetris
        moveTetris(vec2(0, 1));
        break;
    case GLUT_KEY_LEFT:
        moveTetris(vec2(-1, 0));    
        // move left the tetris
        break;
    case GLUT_KEY_RIGHT:
        moveTetris(vec2(1,0));
        // move right the tetris
        break;
    }
}

/**
 * 
 * restart the game
 * */
void restart()
{
    for (int i = 0; i < GRIDROWS; i++)
    {
        for (int j = 0; j < GRIDCOLS; j++)
        {
            // update the color of each unit
        }
    }
}

/**
 * q: quit
 * r: restart
 * */
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'q':
        exit(EXIT_SUCCESS); // quit the game
        break;
    case 'r':
        restart(); // restart the game
        break;
    }
    glutPostRedisplay();
}

/*
 *  
 * 
 * */
void autodown(int value){
    moveTetris(vec2(0,1));

    glutTimerFunc(speed, autodown, 0);
}

/*
 *  
 * 
 * */
void idle(void)
{
    glutPostRedisplay();
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

    glutTimerFunc(speed, autodown, 0);

    glutReshapeFunc(reshape);
    glutSpecialFunc(special);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    glutMainLoop();

    return 0;
}