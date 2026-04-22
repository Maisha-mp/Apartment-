#ifdef _WIN32
#include <windows.h>
#endif

#if defined(__has_include)
#if __has_include(<GL/freeglut.h>)
#include <GL/freeglut.h>
#elif __has_include(<GL/glut.h>)
#include <GL/glut.h>
#else
#error FreeGLUT/GLUT header not found.
#endif
#else
#include <GL/glut.h>
#endif

#include <cmath>
#include <algorithm>
#include <cstdlib>

//Maisha: DDA


const int   WIDTH  = 1000;
const int   HEIGHT = 700;
const float PI     = 3.14159265358979323846f;

float cloudOffset = 0.0f;
float secondAngle = -90.0f;
float minuteAngle =  20.0f;
float hourAngle   =  90.0f;
float treeAngle   =   0.0f;
float ballX       = 290.0f;
float ballSpin    =   0.0f;
float ballDir     =   1.0f;
bool  animateScene = true;

int roundInt(float v)
{
    return (v >= 0.0f) ? static_cast<int>(v + 0.5f)
           : static_cast<int>(v - 0.5f);
}
void plotPoint(int x, int y)
{
    glVertex2i(x, y);
}

void drawLineDDA(int x1, int y1, int x2, int y2)
{
    int dx    = x2 - x1;
    int dy    = y2 - y1;
    int steps = std::max(std::abs(dx), std::abs(dy));
    if (steps == 0)
    {
        glBegin(GL_POINTS);
        plotPoint(x1, y1);
        glEnd();
        return;
    }
    float xInc = dx / static_cast<float>(steps);
    float yInc = dy / static_cast<float>(steps);
    float x    = static_cast<float>(x1);
    float y    = static_cast<float>(y1);
    glBegin(GL_POINTS);
    for (int i = 0; i <= steps; ++i)
    {
        plotPoint(roundInt(x), roundInt(y));
        x += xInc;
        y += yInc;
    }
    glEnd();
}

void plotCirclePoints(int xc, int yc, int x, int y)
{
    plotPoint(xc+x, yc+y);
    plotPoint(xc-x, yc+y);
    plotPoint(xc+x, yc-y);
    plotPoint(xc-x, yc-y);
    plotPoint(xc+y, yc+x);
    plotPoint(xc-y, yc+x);
    plotPoint(xc+y, yc-x);
    plotPoint(xc-y, yc-x);
}

// Maisha: Midpoint circle

void drawCircleMidpoint(int xc, int yc, int r)
{
    int x = 0, y = r, d = 1 - r;
    glBegin(GL_POINTS);
    while (x <= y)
    {
        plotCirclePoints(xc, yc, x, y);
        if (d < 0) d += 2*x + 3;
        else
        {
            d += 2*(x-y) + 5;
            --y;
        }
        ++x;
    }
    glEnd();
}

//Kabir: Bresenham Line

void drawLineBresenham(int x1, int y1, int x2, int y2)
{
    int dx  = std::abs(x2 - x1);
    int dy  = std::abs(y2 - y1);
    int sx  = (x1 < x2) ? 1 : -1;
    int sy  = (y1 < y2) ? 1 : -1;
    int err = dx - dy;
    glBegin(GL_POINTS);
    while (true)
    {
        plotPoint(x1, y1);
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy)
        {
            err -= dy;
            x1 += sx;
        }
        if (e2 <  dx)
        {
            err += dx;
            y1 += sy;
        }
    }
    glEnd();
}

void lineRectBresenham(int x1, int y1, int x2, int y2)
{
    drawLineBresenham(x1,y1,x2,y1);
    drawLineBresenham(x2,y1,x2,y2);
    drawLineBresenham(x2,y2,x1,y2);
    drawLineBresenham(x1,y2,x1,y1);
}

// Maisha: basic shapes

void filledRect(float x1, float y1, float x2, float y2, float r, float g, float b)
{
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(x1,y1);
    glVertex2f(x2,y1);
    glVertex2f(x2,y2);
    glVertex2f(x1,y2);
    glEnd();
}

void filledPolygon4(float x1, float y1, float x2, float y2,float x3, float y3, float x4, float y4, float r,  float g,  float b)
{
    glColor3f(r, g, b);
    glBegin(GL_POLYGON);
    glVertex2f(x1,y1);
    glVertex2f(x2,y2);
    glVertex2f(x3,y3);
    glVertex2f(x4,y4);
    glEnd();
}

void filledCircle(float cx, float cy, float radius,float r, float g, float b)
{
    glColor3f(r, g, b);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= 120; ++i)
    {
        float ang = 2.0f * PI * i / 120.0f;
        glVertex2f(cx + radius * std::cos(ang),
                   cy + radius * std::sin(ang));
    }
    glEnd();
}

void lineRectDDA(int x1, int y1, int x2, int y2)
{
    drawLineDDA(x1,y1,x2,y1);
    drawLineDDA(x2,y1,x2,y2);
    drawLineDDA(x2,y2,x1,y2);
    drawLineDDA(x1,y2,x1,y1);
}

// Maisha: drawFloor

void drawFloor()
{
    filledRect(0, 0, WIDTH, 220, 0.89f, 0.83f, 0.71f);

    glColor3f(0.82f, 0.76f, 0.64f);
    glPointSize(1.0f);
    drawLineDDA(0, 145, WIDTH, 145);
    drawLineDDA(0,  75, WIDTH,  75);
    drawLineDDA(0,  55, WIDTH,  55);
}

// Maisha: drawCloud

void drawCloud(float tx, float ty, float scale)
{
    glPushMatrix();
    glTranslatef(tx, ty, 0.0f);
    glScalef(scale, scale, 1.0f);
    filledCircle( 0,  0, 22, 1.0f, 1.0f, 1.0f);
    filledCircle(22, 10, 18, 1.0f, 1.0f, 1.0f);
    filledCircle(42,  6, 20, 1.0f, 1.0f, 1.0f);
    filledCircle(60,  2, 14, 1.0f, 1.0f, 1.0f);
    glPopMatrix();
}

void drawTree(float tx, float ty, float scale);

//Maisha: drawWindowUnit

void drawWindowUnit()
{

    filledRect(0,   0, 270, 380, 0.72f, 0.58f, 0.38f);
    filledRect(10, 12, 260, 368, 0.78f, 0.87f, 0.96f);
    filledRect(10, 12, 260,  58, 0.52f, 0.66f, 0.36f);
    drawTree(178, 118, 1.15f);
    drawCloud(38 + cloudOffset, 300, 1.2f);
    glColor3f(0.55f, 0.42f, 0.26f);
    glPointSize(2.0f);
    drawLineDDA(135,  12, 135, 368);
    drawLineDDA( 10, 180, 260, 180);
    glColor3f(0.52f, 0.39f, 0.24f);
    lineRectDDA(0, 0, 270, 380);
}

//Maisha: update() animation timer (~30 fps)

void update(int value)
{
    (void)value;
    if (animateScene)
    {


        cloudOffset += 0.35f;
        if (cloudOffset > 78.0f) cloudOffset = -18.0f;
        secondAngle -= 0.2f;
        minuteAngle -= 0.2f / 60.0f;
        hourAngle   -= 0.2f / 720.0f;
        treeAngle = 4.0f * std::sin(cloudOffset * 0.06f);
        ballX    += 1.2f * ballDir;
        ballSpin -= 3.5f * ballDir;
        if (ballX > 590.0f) ballDir = -1.0f;
        if (ballX < 230.0f) ballDir =  1.0f;
    }
    glutPostRedisplay();
    glutTimerFunc(33, update, 0);
}

//Maisha : init() + main()

void init()
{
    glClearColor(0.97f, 0.96f, 0.94f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, static_cast<GLdouble>(WIDTH),
               0.0, static_cast<GLdouble>(HEIGHT));
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPointSize(2.0f);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(60, 40);
    glutCreateWindow("Peaceful Apartment - Computer Graphics Project");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, update, 0);
    glutMainLoop();
    return 0;
}
