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

int roundInt(float v) {
    return (v >= 0.0f) ? static_cast<int>(v + 0.5f)
                       : static_cast<int>(v - 0.5f);
}
void plotPoint(int x, int y) { glVertex2i(x, y); }

void drawLineDDA(int x1, int y1, int x2, int y2) {
    int dx    = x2 - x1;
    int dy    = y2 - y1;
    int steps = std::max(std::abs(dx), std::abs(dy));
    if (steps == 0) {
        glBegin(GL_POINTS); plotPoint(x1, y1); glEnd();
        return;
    }
    float xInc = dx / static_cast<float>(steps);
    float yInc = dy / static_cast<float>(steps);
    float x    = static_cast<float>(x1);
    float y    = static_cast<float>(y1);
    glBegin(GL_POINTS);
    for (int i = 0; i <= steps; ++i) {
        plotPoint(roundInt(x), roundInt(y));
        x += xInc;
        y += yInc;
    }
    glEnd();
}
