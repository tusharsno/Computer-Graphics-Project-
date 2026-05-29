#include "ui.h"
#include "planet.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cstdio>

bool showLabels    = true;
bool showOrbits    = true;
int  selectedPlanet = -1;
float currentSpeed  = 1.0f;
bool  isPaused      = false;
bool  telescopeMode = false;

static int winW = 1200, winH = 700;

void drawText(float x, float y, const char* text) {
    glRasterPos2f(x, y);
    for (const char* c = text; *c; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
}

void drawHUD() {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, winW, 0, winH);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    glColor3f(1, 1, 1);
    drawText(10, winH - 20, "3D Solar System Simulation");
    drawText(10, winH - 38, "Mouse Drag: Rotate | Scroll/W/S: Zoom | SPACE: Pause | =/- : Speed | T: Telescope");
    drawText(10, winH - 54, "O: Orbits | L: Labels | R: Reset | Double-Click: Focus | F1: Screenshot | ESC: Exit");

    // speed indicator
    char speedBuf[32];
    if (isPaused) {
        glColor3f(1.0f, 0.4f, 0.4f);
        drawText(10, winH - 72, "[ PAUSED ]");
    } else {
        snprintf(speedBuf, sizeof(speedBuf), "Speed: %.2fx", currentSpeed);
        if      (currentSpeed > 3.0f) glColor3f(1.0f, 0.5f, 0.2f);
        else if (currentSpeed < 0.5f) glColor3f(0.5f, 0.8f, 1.0f);
        else                          glColor3f(0.4f, 1.0f, 0.4f);
        drawText(10, winH - 72, speedBuf);
    }

    if (telescopeMode) {
        glColor3f(0.4f, 0.9f, 1.0f);
        drawText(10, winH - 90, "[ TELESCOPE MODE ]");
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void drawInfoPanel(int idx) {
    if (idx < 0 || idx >= 9) return;

    static const char* periods[]  = {"88 days","225 days","365 days","687 days","12 years","29 years","84 years","165 years","248 years"};
    static const float distances[] = {0.39f, 0.72f, 1.0f, 1.52f, 5.2f, 9.58f, 19.2f, 30.05f, 39.5f};
    static const char* types[]    = {"Planet","Planet","Planet","Planet","Planet","Planet","Planet","Planet","Dwarf Planet"};

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, winW, 0, winH);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    // panel background
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.0f, 0.2f, 0.75f);
    glBegin(GL_QUADS);
        glVertex2f(winW - 210, 10);
        glVertex2f(winW - 10,  10);
        glVertex2f(winW - 10,  100);
        glVertex2f(winW - 210, 100);
    glEnd();
    glDisable(GL_BLEND);

    glColor3f(1.0f, 0.9f, 0.3f);
    char buf[64];
    snprintf(buf, sizeof(buf), "%s: %s", types[idx], planets[idx].name.c_str());
    drawText(winW - 200, 82, buf);
    glColor3f(1, 1, 1);
    snprintf(buf, sizeof(buf), "Distance: %.2f AU", distances[idx]);
    drawText(winW - 200, 64, buf);
    snprintf(buf, sizeof(buf), "Orbital Period: %s", periods[idx]);
    drawText(winW - 200, 46, buf);
    snprintf(buf, sizeof(buf), "Double-click to focus");
    drawText(winW - 200, 28, buf);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}
