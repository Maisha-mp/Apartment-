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

