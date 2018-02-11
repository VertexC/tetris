// Two-Dimensional Sierpinski Gasket
// Generated using randomly selected vertices and bisection

#include "Angel.h"
#include <stdio.h>
#include "Tetris.h"
//It is nice to define and use a color pallete on your software. You can put this information on another utility file.
static vec3 base_colors[] = {
    vec3(1.0, 0.0, 0.0),
    vec3(0.0, 1.0, 0.0),
    vec3(0.0, 0.0, 1.0),
    vec3(0.0, 0.0, 0.0)};

//three triangles
const int NumPoints = 9;

//and two lines
const int NumPoints1 = 4;

// for grid
const int NumGridPorints = (21 + 11) * 2; //
//vertex array for the triangles and for the lines, respectively
GLuint vao, vao1;

//triangle points and colors arrays
vec2 points[NumPoints];
vec3 colors[NumPoints];

//lines points and colors arrays
vec2 points1[NumPoints1];
vec3 colors1[NumPoints1];
//----------------------------------------------------------------------------



class
//grid points and color arrays
vec2 gridPoints[NumGridPorints];
vec3 gridColors[NumGridPorints];
// maybe generate a class of it
// size of window
int widthWindow;
int heightWindow;

// border of window
int widthborder;
int heightborder;
// size of every small squre
int unitLength;

void setParameters()
{
    unitLength = 40;

    widthborder = unitLength;
    heightborder = unitLength;

    widthWindow = 10 * unitLength + widthborder;
    heightWindow = 20 * unitLength + heightborder;
}

void init(void)
{
    //Just some hard coded data
    float widthPerUnit = 2.0 / 10;
    float heightPerUnit = 2.0 / 20;

    // set up the grid
    // set up the col of grid
    for (int i = 0; i < 11; i++)
    {
        gridPoints[2 * i] = vec2(-1.0, 1.0) + vec2(i * widthPerUnit, 0.0);
        gridPoints[2 * i + 1] = vec2(-1.0, -1.0) + vec2(i * widthPerUnit, 0.0);
    }
    // set up the row of grid
    for (int j = 0; j < 21; j++)
    {
        gridPoints[22 + 2 * j] = vec2(-1.0, 1.0) - vec2(0.0, j * heightPerUnit);
        gridPoints[22 + 2 * j + 1] = vec2(1.0, 1.0) - vec2(0.0, j * heightPerUnit);
    }
    // set up the color of grid
    for (int i = 0; i < (21 + 11) * 2; i++)
    {
        gridColors[i] = base_colors[3];
    }

    // Vertex positions for three triangles
    // Three triangles forming a simple Gasket
    points[0] = vec2(-1.0, -1.0);
    points[1] = vec2(0.0, -1.0);
    points[2] = vec2(-0.5, 0.0);

    points[3] = vec2(0.0, -1.0);
    points[4] = vec2(0.5, 0.0);
    points[5] = vec2(1.0, -1.0);

    points[6] = vec2(0.0, 1.0);
    points[7] = vec2(-0.5, 0.0);
    points[8] = vec2(0.5, 0.0);

    //color stuff for each vertex of each of the triangles
    colors[0] = base_colors[0];
    colors[1] = base_colors[0];
    colors[2] = base_colors[1];

    colors[3] = base_colors[1];
    colors[4] = base_colors[1];
    colors[5] = base_colors[2];

    colors[6] = base_colors[2];
    colors[7] = base_colors[2];
    colors[8] = base_colors[0];
    //***************************

    //line starting points and ending points. The first two positions represent one line segment,
    //while the last two positions represent another line segment.
    points1[0] = vec2(-0.5, 0.0);
    points1[1] = vec2(-1.0, 1.0);

    points1[2] = vec2(-1.0, -1.0);
    points1[3] = vec2(1.0, 1.0);

    //lines can have color too!
    colors1[0] = base_colors[3];
    colors1[1] = base_colors[3];
    colors1[2] = base_colors[3];
    colors1[3] = base_colors[3];

    //Here we create another vertexArrayObject to render some lines. This is intended to be your grid, so since the
    //grid positions never change, you can leave this vertex array object on the initialization.
    // Create and bind a vertex array object
    glGenVertexArrays(1, &vao1);
    glBindVertexArray(vao1);

    // Create and initialize a buffer object
    GLuint buffer1;
    glGenBuffers(1, &buffer1);
    glBindBuffer(GL_ARRAY_BUFFER, buffer1);

    // glBufferData( GL_ARRAY_BUFFER, sizeof(points1), points1, GL_STATIC_DRAW );
    glBufferData(GL_ARRAY_BUFFER, sizeof(gridPoints) + sizeof(gridColors), gridPoints, GL_STATIC_DRAW);

    // glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points1), points1);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(gridPoints), gridPoints);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(gridPoints), sizeof(gridColors), gridColors);

    // Load shaders and use the resulting shader program
    GLuint program1 = InitShader("vshader.glsl", "fshader.glsl");
    glUseProgram(program1);

    // Initialize the vertex position attribute from the vertex shader
    GLuint loc1 = glGetAttribLocation(program1, "vPosition");
    glEnableVertexAttribArray(loc1);
    glVertexAttribPointer(loc1, 2, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(0));

    GLuint vColor1 = glGetAttribLocation(program1, "vColor");
    glEnableVertexAttribArray(vColor1);
    glVertexAttribPointer(vColor1, 3, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(sizeof(gridPoints)));

    // // glBufferData( GL_ARRAY_BUFFER, sizeof(points1), points1, GL_STATIC_DRAW );
    // glBufferData(GL_ARRAY_BUFFER, sizeof(points1) + sizeof(colors1), points1, GL_STATIC_DRAW);

    // glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points1), points1);
    // glBufferSubData(GL_ARRAY_BUFFER, sizeof(points1), sizeof(colors1), colors1);

    // // Load shaders and use the resulting shader program
    // GLuint program1 = InitShader("vshader.glsl", "fshader.glsl");
    // glUseProgram(program1);

    // //Initialize the vertex position attribute from the vertex shader
    //     GLuint loc1 = glGetAttribLocation(program1, "vPosition");
    // glEnableVertexAttribArray(loc1);
    // glVertexAttribPointer(loc1, 2, GL_FLOAT, GL_FALSE, 0,
    //                       BUFFER_OFFSET(0));

    // GLuint vColor1 = glGetAttribLocation(program1, "vColor");
    // glEnableVertexAttribArray(vColor1);
    // glVertexAttribPointer(vColor1, 3, GL_FLOAT, GL_FALSE, 0,
    //                       BUFFER_OFFSET(sizeof(points1)));
    //****************************
    glClearColor(1.0, 1.0, 1.0, 1.0); // white background
}

//----------------------------------------------------------------------------

void display(void)
{
    // Create and bind a vertex array object
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create and bind a buffer object
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    //glBufferData( GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW );
    glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors), points, GL_STATIC_DRAW);

    // Next, we load the real data in parts.  We need to specify the
    //   correct byte offset for placing the color data after the point
    //   data in the buffer.  Conveniently, the byte offset we need is
    //   the same as the size (in bytes) of the points array, which is
    //   returned from "sizeof(points)".
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);

    // Load shaders and use the resulting shader program
    GLuint program = InitShader("vshader.glsl", "fshader.glsl");
    glUseProgram(program);

    // Initialize the vertex position attribute from the vertex shader
    // Keep in mind that the second parameter is associated with the length of the array sent to opengl (1D,2D,3D or 4D).
    // The last parameter is the offset where the data is stored on the buffer.
    GLuint loc = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(0));

    // Likewise, initialize the vertex color attribute.  Once again, we
    //    need to specify the starting offset (in bytes) for the color
    //    data.  Just like loading the array, we use "sizeof(points)"
    //    to determine the correct value.
    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(sizeof(points)));

    glClear(GL_COLOR_BUFFER_BIT); // clear the window

    //Draw triangles
    //Here we are binding back the first vertex array object. Now we can acess all the buffers associated to it and render accordingly
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, NumPoints);

    //Draw lines using the second vertex array object. On your tetris code, you probabily want to draw the lines first, then the triangles.
    //If you want to change the thickness of the lines, this is how:  glLineWidth(5.0);
    glLineWidth(5.0f); //GL LineWidth doesn't work                     
    glBindVertexArray(vao1);
    glDrawArrays(GL_LINES, 0, NumGridPorints);
    //Causes all issued commands to be executed as quickly as they are accepted by the actual rendering engine
    glFlush();
}

//----------------------------------------------------------------------------

//timed function. We intended to execute this every one second.
void rotateDelay(int)
{
    //mat2 constructor is defined as follows
    //mat 2 ( GLfloat m00, GLfloat m10, GLfloat m01, GLfloat m11 )

    //We can define a rotation angle in Radians
    GLfloat angle = M_PI / 4;

    //This is the 2x2 rotation matrix for 2D points.
    static mat2 rotate10degccw = mat2(cos(angle), sin(angle), -sin(angle), cos(angle));

    //The rotation transform is applied using the origin as the rotation axis, if you want a different axis you need to define yourself.
    //in this case we are just using the center of the triangle
    vec2 center = vec2(0.0, 0.0);

    for (int i = 3; i < 6; ++i)
    {
        center += points[i];
    }

    center = center / 3;

    for (int i = 3; i < 6; ++i)
    {
        //finally, we apply the transformation. The idea here is just to translate the points to origin, rotate and then translate it back
        points[i] = rotate10degccw * (points[i] - center) + center;
    }

    //Always remember to update your canvas
    glutPostRedisplay();

    //then we can set another identical event in 1000 miliseconds in the future, that is how we keep the triangle rotating
    glutTimerFunc(1000.0, rotateDelay, 0);
}

//If you want to use arrows, take a look at this function
//It works similarly, but you have no acess to the ASCII keys using it. Please see the link below for details.
//https://www.opengl.org/resources/libraries/glut/spec3/node54.html
void keyboardSpecial(int key, int x, int y)
{
    switch (key)
    {
    //similar to the stuff inside the keyboard class, but applying the translaton only to the third triangle.
    case GLUT_KEY_DOWN:
        printf("down arrow pressed!\n");
        static vec2 downStep = vec2(0.0, -0.1);

        for (int i = 6; i < 9; ++i)
        {
            points[i] = points[i] + downStep;
        }
        glutPostRedisplay();
        break;
    case 033:
        exit(EXIT_SUCCESS);
        break;
    }
}

//bind most keys on your keyboard to this function
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 115:
        printf("w pressed!\n");

        //this is just a simple 2D array that represents a vector pointing downwards
        static vec2 downStep = vec2(0.0, -0.4);

        //you can apply this transformation to specific points, in this case, the vertices of the first triangle
        for (int i = 0; i < 3; ++i)
        {
            //notice that you can just sum 2D arrays directly! This makes things more easy.
            points[i] = points[i] + downStep;
        }
        //Always remember to call glutpostRedisplay after changes. It will just call the "display" method again.
        glutPostRedisplay();
        break;

    //Esc pressed!
    case 033:
        exit(EXIT_SUCCESS);
        break;
    }
}

//----------------------------------------------------------------------------

int main(int argc, char **argv)
{
    setParameters();

    glutInit(&argc, argv);
    //We explicitly tell OpenGL that we are using the RGBA mode. When we start working with 3D resources there will be changes here.
    glutInitDisplayMode(GLUT_RGBA);

    //Change the size of the screen here. Keep in mind that the coordinates will also change accordingly
    //so, if you want to keep your points on the rendering canvas undisturbed you need a square screen.
    glutInitWindowSize(widthWindow, heightWindow);

    // If you are using freeglut, the next two lines will check if
    // the code is truly 3.2. Otherwise, comment them out
    glutInitContextVersion(3, 2);
    glutInitContextProfile(GLUT_CORE_PROFILE);

    //Title of your screen, change accordingly
    glutCreateWindow("Sierpinski Gasket");

    // Iff you get a segmentation error at line 34, please uncomment the line below
    glewExperimental = GL_TRUE;
    glewInit();

    //this is the initialization function for your data.
    init();

    //Here we bind the function "display" to the OpenGL handler that will render your data
    glutDisplayFunc(display);

    //Similarly, we can also bind a function that will be executed 1000 miliseconds later. We bind this to the function rotateDelay.
    // We then bind this to the function "rotateDelay". The 0 in the end is the input of the rotateDelay class, it can only be an integer.
    glutTimerFunc(1000.0, rotateDelay, 0);

    //Finally, we bind the keyboard events inside the class "keyboard" using the line below
    glutKeyboardFunc(keyboard);

    //If you really want to use F1 ~ F9 keys, or the arrow keys, then use the line of code below.
    glutSpecialFunc(keyboardSpecial);

    glutMainLoop();
    return 0;
}
