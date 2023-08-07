/*********
   CTIS164 - Template Source Program
----------
STUDENT : Davut Durmaz
SECTION : 164 - 4
HOMEWORK: 1st 164 Homework
----------
PROBLEMS: I think ý met all requirements written on the homework guide pdf. However, ý am not sure whether it is wanted or not that, on AUTONOMOUS mode, whenever user clicks on the places near to the border of the window, program outomatically sends the shape inside of window. That is, shape cannot escape/disappear over borders of the screen. Another point is that, on MANUEL mode, whenever user clicks on the places near to the border of the window, some parts of the complex shape go outside of the screen.
----------
ADDITIONAL FEATURES: Additonal Features Are: 1) Changing colour of some parts of the plane depending on the changing x and y axis values.
                                             2) Moving clouds on the background. Whenever plane goes RIGHT or LEFT side, clouds go reverse direction depending.
                                             3) I added an laser effect. Whenever user clicks right button of the mouse, some laser effects are released. If user keep holding the right mouse click, laser effect is on display. Whenever user release right mouse button, laser disappears.
                                             4) While running the program, if user click on left mouse button and drag the mouse, shape is also dragged and position is changed by the help of dragging mouse event. This is available for both modes.
*********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include<time.h>

#define WINDOW_WIDTH  1000
#define WINDOW_HEIGHT 600

#define TIMER_PERIOD  50 // Period for the timer.
#define TIMER_ON         0 // 0:disable timer, 1:enable timer

#define D2R 0.0174532


/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

//bool object = false;
int count = 0;   // To determine state of initial screen, initailly, there is no shape on the screen. However, whenever count value changes screen changes.
int xAxis = 0, yAxis = 0;  // To modify glut library coordinat system for new coordinat system, proportions of new screen.
int r = 180, g = 172, b = 50;  // Initialized RGB color values.
bool mode = true;   // Mode value true/false in order to determine which mode we want to use.
char direction;   // To keep direction info in order to manage actions and display some info about direction on the screen.
bool activeTimer = false; // For state of timer ON/OFF.
int counter = 0;
int cloudPosition = 0;  // Initialized cloud position on x axis. Whenever cloudPosition info changes, clouds can go left or right depending on movement of plane shape.
bool laserOn = false;  // For state of laser ON/OFF.


//bool location(int x, int y) {
//    return (x > 150 && x < 450 && y > 100 && y < 300);
//}

//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
    float angle;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
    float angle;

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void print(int x, int y, const char* string, void* font)
{
    int len, i;

    glRasterPos2f(x, y);
    len = (int)strlen(string);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, string[i]);
    }
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void* font, const char* string, ...)
{
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);

    int len, i;
    glRasterPos2f(x, y);
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, str[i]);
    }
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char* string, ...) {
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(size, size, 1);

    int len, i;
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
    }
    glPopMatrix();
}



//CLOUDS ON BACKGROUND SCREEN 
void clouds(int x)  /// Cloud shape code segment
{

    glColor3f(1, 1, 1);
    circle(-270 + x, 250, 30);
    circle(-270 + x, 200, 30);
    circle(-300 + x, 230, 30);
    circle(-240 + x, 230, 30);

    circle(0 + x, 180, 30);
    circle(0 + x, 130, 30);
    circle(-30 + x, 160, 30);
    circle(30 + x, 160, 30);

    circle(250 + x, 250, 30);
    circle(250 + x, 200, 30);
    circle(280 + x, 230, 30);
    circle(220 + x, 230, 30);

    circle(500 + x, 180, 30);
    circle(500 + x, 130, 30);
    circle(470 + x, 160, 30);
    circle(530 + x, 160, 30);

    circle(-520 + x, 180, 30);
    circle(-520 + x, 130, 30);
    circle(-550 + x, 160, 30);
    circle(-485 + x, 160, 30);
}

void laser(int x, int y)   /// Laser effect code segment
{
    glColor3ub(r, g, b);
    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex2f(-75 + x, 50 + y);
    glVertex2f(-55 + x, 70 + y);

    glVertex2f(-75 + x, 50 + y);
    glVertex2f(-95 + x, 70 + y);

    glVertex2f(-75 + x, 50 + y);
    glVertex2f(-65 + x, 70 + y);

    glVertex2f(-75 + x, 50 + y);
    glVertex2f(-85 + x, 70 + y);

    glVertex2f(-75 + x, 50 + y);
    glVertex2f(-75 + x, 300 + y);

    glVertex2f(75 + x, 50 + y);
    glVertex2f(95 + x, 70 + y);

    glVertex2f(75 + x, 50 + y);
    glVertex2f(55 + x, 70 + y);

    glVertex2f(75 + x, 50 + y);
    glVertex2f(85 + x, 70 + y);

    glVertex2f(75 + x, 50 + y);
    glVertex2f(65 + x, 70 + y);

    glVertex2f(75 + x, 50 + y);
    glVertex2f(75 + x, 300 + y);


    glEnd();

}

void shape(int x, int y)   /// Plane shape code segment
{

    glColor3ub(r, g, b);
    circle(x, 70 + y, 25);

    glColor3f(1, 0, 0);
    glRectf(-25 + x, -80 + y, 25 + x, 70 + y);

    glColor3ub(51, 153, 255);
    glRectf(-85 + x, -30 + y, -65 + x, 40 + y);
    glRectf(65 + x, -30 + y, 85 + x, 40 + y);
    glColor3f(0.4, 0.8, 0.7);
    glRectf(-78 + x, 40 + y, -72 + x, 50 + y);
    glRectf(72 + x, 40 + y, 78 + x, 50 + y);

    glColor3ub(255, 140, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(-125 + x, -30 + y);
    glVertex2f(-25 + x, -30 + y);
    glVertex2f(-25 + x, 50 + y);

    glVertex2f(125 + x, -30 + y);
    glVertex2f(25 + x, -30 + y);
    glVertex2f(25 + x, 50 + y);
    glEnd();

    glColor3ub(g, r, b);
    glRectf(-25 + x, -50 + y, -125 + x, -30 + y);
    glRectf(125 + x, -50 + y, 25 + x, -30 + y);

    glColor3ub(b, g, r);
    glBegin(GL_TRIANGLES);
    glVertex2f(-25 + x, -80 + y);
    glVertex2f(25 + x, -80 + y);
    glVertex2f(-15 + x, -110 + y);

    glVertex2f(-25 + x, -80 + y);
    glVertex2f(25 + x, -80 + y);
    glVertex2f(15 + x, -110 + y);

    glColor3f(1, 1, 0);
    glVertex2f(-25 + x, -80 + y);
    glVertex2f(25 + x, -80 + y);
    glVertex2f(0 + x, -115 + y);
    glEnd();

    glColor3ub(r, r, b);
    circle(0 + x, 40 + y, 15);
    circle(0 + x, 0 + y, 15);
    circle(0 + x, -40 + y, 15);

}

//
// To display onto window using OpenGL commands
//
void display() {
    //
    // clear window to black
    //
    glClearColor(0.7, 0.3, 0.9, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1, 1, 1);


    clouds(cloudPosition);  /// To display clouds background


    glColor3f(0, 0, 0);
    if (count == 0)
        vprint(-150, 0, GLUT_BITMAP_9_BY_15, "Click to create/relocate the Shape");   /// Initial information for user
    else
    {
        shape(xAxis, yAxis);   /// To display plane shape on the screen

        if (laserOn == true)
            laser(xAxis, yAxis);   /// To give some laser shooting effect 
    }


    glColor3f(0, 0, 0);
    if (mode == true)
    {
        vprint(-480, 280, GLUT_BITMAP_9_BY_15, "Mode : ");   /// To display mode information
        vprint(-420, 280, GLUT_BITMAP_9_BY_15, "MANUAL");
    }
    else
    {
        vprint(-480, 280, GLUT_BITMAP_9_BY_15, "Mode : ");  /// To display mode information
        vprint(-420, 280, GLUT_BITMAP_9_BY_15, "AUTONOMOUS");
    }

    switch (direction)  /// To display which direction the plane goes over at the left upper corner
    {
    case 'U':
        vprint(-480, 265, GLUT_BITMAP_9_BY_15, "Direction : ");
        vprint(-375, 265, GLUT_BITMAP_9_BY_15, "UP");
        break;
    case 'D':
        vprint(-480, 265, GLUT_BITMAP_9_BY_15, "Direction : ");
        vprint(-375, 265, GLUT_BITMAP_9_BY_15, "DOWN");
        break;
    case 'L':
        vprint(-480, 265, GLUT_BITMAP_9_BY_15, "Direction : ");
        vprint(-375, 265, GLUT_BITMAP_9_BY_15, "LEFT");
        break;
    case 'R':
        vprint(-480, 265, GLUT_BITMAP_9_BY_15, "Direction : ");
        vprint(-375, 265, GLUT_BITMAP_9_BY_15, "RIGHT");
        break;
    default:vprint(-480, 265, GLUT_BITMAP_9_BY_15, "Direction : ");
        vprint(-375, 265, GLUT_BITMAP_9_BY_15, "NOT SET");
        break;
    }

    vprint(380, 275, GLUT_BITMAP_8_BY_13, "DAVUT DURMAZ");  /// Name-Surname
    vprint(380, 260, GLUT_BITMAP_8_BY_13, "21903291");   ///ID

    vprint(-270, -280, GLUT_BITMAP_9_BY_15, "PRESS <F1> TO SWITCH THE MODE AND ARROW KEYS TO CONTROL THE SHAPE");  /// To give some info for user

    glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{

    switch (key)  /// Whenever user click on the F1 button mode is changed. Mode is defaut as MANUAL at the begining but shape cannot move because there is no direction decided.
    {
    case GLUT_KEY_F1:mode = !mode;
    }

    if (mode == true)   /// For true value of mode, mode is MANUAL and user uses arrow keys to increase or decreade x and y values in order to move the shape. In this way, direction is decided to go over.
    {
        activeTimer == 0;
        switch (key) {
        case GLUT_KEY_UP:
            if (yAxis <= (WINDOW_HEIGHT / 2 - 98))
            {
                yAxis = yAxis + 5;
                r = rand() % 256;
                g = rand() % 256;
                b = rand() % 256;
            }
            break;
            direction = 'U';
            break;
        case GLUT_KEY_DOWN:
            if (yAxis >= -(WINDOW_HEIGHT / 2 - 115))
            {
                yAxis = yAxis - 5;
                r = rand() % 256;
                g = rand() % 256;
                b = rand() % 256;
            }
            direction = 'D';
            break;
        case GLUT_KEY_LEFT:
            if (xAxis >= -(WINDOW_WIDTH / 2 - 127))
            {
                xAxis = xAxis - 5;
                r = rand() % 256;
                g = rand() % 256;
                b = rand() % 256;
            }
            direction = 'L';
            break;
        case GLUT_KEY_RIGHT:
            if (xAxis <= (WINDOW_WIDTH / 2 - 127))
            {
                xAxis = xAxis + 5;
                r = rand() % 256;
                g = rand() % 256;
                b = rand() % 256;
            }
            direction = 'R';
            break;
        }
    }

    if (key == GLUT_KEY_F1)  /// To activate or deactivate timer, whenever mode is false, AUTONOMOUS mode, timer is activated and it start or continue counting.
    {
        if (mode == false)
        {
            activeTimer = !activeTimer; //to stop or resume timer.
        }
    }

    switch (key)  /// I have written this code segment to give some effects to clouds on the sky, whenever plane goes left direction clouds go reverse direction. Whenever plane goes right direction, clouds go left direction.
    {
    case GLUT_KEY_UP:direction = 'U'; break;
    case GLUT_KEY_DOWN:direction = 'D'; break;
    case GLUT_KEY_LEFT:direction = 'L'; cloudPosition++; break;
    case GLUT_KEY_RIGHT:direction = 'R'; cloudPosition--; break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = false; break;
    case GLUT_KEY_DOWN: down = false; break;
    case GLUT_KEY_LEFT: left = false; break;
    case GLUT_KEY_RIGHT: right = false; break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
    // Write your codes here.

    if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN) /// I have written these code segment below to initialize the program while click left button of mouse. Whenever user click on left button of mouse shape initialized and displayed on the screen.
    {
        count++;  /// Whenever count is increased, program goes into dynamic mode. While count is "0" nothing is displayed on the screen.
        printf("x = %d   y = %d\n", x, y);
        xAxis = x - WINDOW_WIDTH / 2;   /// I modified the coordinat system of glut library by using these both line of codes
        yAxis = WINDOW_HEIGHT / 2 - y;   /// I modified the coordinat system of glut library by using these both line of codes

        r = rand() % 256;    /// Changing RGB colors for some parts of complex shape displayed on the screen, in each changes of x or y coordinats RGB colors are changing.
        g = rand() % 256;
        b = rand() % 256;
    }

    if (button == GLUT_RIGHT_BUTTON && stat == GLUT_DOWN)  /// These two if statement are written for an laser effect. In the first event, user clicks on right button of mouse to fire the laser.
    {
        laserOn = !laserOn;  /// To change bool value 
    }
    if (button == GLUT_RIGHT_BUTTON && stat == GLUT_UP)   /// In the second event, whenever user release the right button of the mouse, laser is stopped to get fired. Otherwise, same effect is always displayed, which is not my aim.
    {
        laserOn = !laserOn;
    }



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    display(); // refresh window.
}

void onMoveDown(int x, int y) {    //MOVEDOWN event, while holding left click of the mouse down, i can change location of the shape displayed on the screen
    // Write your codes here.

    printf("MOVEDOWN :  x = %d   y = %d\n", x, y);
    xAxis = x - WINDOW_WIDTH / 2;
    yAxis = WINDOW_HEIGHT / 2 - y;

    r = rand() % 256;
    g = rand() % 256;
    b = rand() % 256;

    // to refresh the window it calls display() function   
    glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void onTimer(int v) {

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.

    if (activeTimer)   /// I modified the timer for my need. I declared a variable for activeTimer instead of using a constant value and counter is for both showing the time and using in some cases
        counter++;



    if (mode == false)   /// mode = false represents AUTONOMOUS mode, then shape can move on last direction
    {
        switch (direction)
        {
        case 'U':   /// If direction is UP, increase value of y axis
            if (yAxis <= (WINDOW_HEIGHT / 2 - 98))
            {
                yAxis = yAxis + 5;
                r = rand() % 256;
                g = rand() % 256;
                b = rand() % 256;
            }
            break;
        case 'D':    /// If direction is DOWN, decrease value of y axis
            if (yAxis >= -(WINDOW_HEIGHT / 2 - 115))
            {
                yAxis = yAxis - 5;
                r = rand() % 256;
                g = rand() % 256;
                b = rand() % 256;
            }
            break;
        case 'R':    /// If direction is RIGHT, increase value of x axis
            if (xAxis <= (WINDOW_WIDTH / 2 - 127))
            {
                cloudPosition--;
                xAxis = xAxis + 5;
                r = rand() % 256;
                g = rand() % 256;
                b = rand() % 256;
            }
            break;
        case 'L':   /// If direction is LEFT, decrease value of x axis
            if (xAxis >= -(WINDOW_WIDTH / 2 - 127))
            {
                cloudPosition++;
                xAxis = xAxis - 5;
                r = rand() % 256;
                g = rand() % 256;
                b = rand() % 256;
            }
            break;
        }
    }

    if (mode == false)  /// I have written these codes below in order to change the direction of the shape on the screen. Whenever the shape reaches to any of side of the screen, program returns its direction and it starts moving on reverse direction
    {
        if (yAxis >= (WINDOW_HEIGHT / 2 - 98))
        {
            direction = 'D';
            if (yAxis != -(WINDOW_HEIGHT / 2 - 115))
            {
                yAxis = yAxis - 5;
                r = rand() % 256;
                g = rand() % 256;
                b = rand() % 256;
            }
        }
        if (yAxis <= -(WINDOW_HEIGHT / 2 - 115))
        {
            direction = 'U';
            if (yAxis != (WINDOW_HEIGHT / 2 - 98))
            {
                yAxis = yAxis + 5;
                r = rand() % 256;
                g = rand() % 256;
                b = rand() % 256;
            }
        }
        if (xAxis >= (WINDOW_WIDTH / 2 - 127))
        {
            direction = 'L';
            if (xAxis != -(WINDOW_WIDTH / 2 - 127))
            {
                cloudPosition++;
                xAxis = xAxis - 5;
                r = rand() % 256;
                g = rand() % 256;
                b = rand() % 256;
            }
        }
        if (xAxis <= -(WINDOW_WIDTH / 2 - 127))
        {
            direction = 'R';
            if (xAxis != (WINDOW_WIDTH / 2 - 127))
            {
                cloudPosition--;
                xAxis = xAxis + 5;
                r = rand() % 256;
                g = rand() % 256;
                b = rand() % 256;
            }
        }
    }

    // to refresh the window it calls display() function
    glutPostRedisplay(); // display()

}

void Init() {

    // Smoothing shapes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    //glutInitWindowPosition(100, 100);
    glutCreateWindow("1st 164 Homework  //  Davut Durmaz");

    glutDisplayFunc(display);
    glutReshapeFunc(onResize);

    srand(time(NULL));

    //
    // keyboard registration
    //
    glutKeyboardFunc(onKeyDown);
    glutSpecialFunc(onSpecialKeyDown);

    glutKeyboardUpFunc(onKeyUp);
    glutSpecialUpFunc(onSpecialKeyUp);

    //
    // mouse registration
    //
    glutMouseFunc(onClick);
    glutMotionFunc(onMoveDown);
    glutPassiveMotionFunc(onMove);


    // timer event
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);


    Init();

    glutMainLoop();
}