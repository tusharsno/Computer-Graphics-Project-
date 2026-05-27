#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cmath>
#include <cstdio>

#include "src/planet.h"
#include "src/renderer.h"
#include "src/camera.h"
#include "src/lighting.h"
#include "src/texture.h"
#include "src/ui.h"
#include "src/audio.h"

static bool  paused     = false;
static float speedMult  = 1.0f;
static int   winW       = 1200;
static int   winH       = 700;

// ── physics update ──────────────────────────────────────────────────────────
void update(int) {
    currentSpeed = speedMult;
    isPaused     = paused;
    if (!paused) {
        for (int i = 0; i < planetCount; i++) {
            planets[i].orbitAngle    += planets[i].orbitSpeed    * speedMult;
            planets[i].rotationAngle += planets[i].rotationSpeed * speedMult * 10.0f;
            if (planets[i].hasMoon)
                planets[i].moonAngle += planets[i].moonOrbitSpeed * speedMult;
        }
    }
    smoothUpdateCamera();
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);   // ~60 FPS
}

// ── display ──────────────────────────────────────────────────────────────────
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    applyCamera();

    renderScene();

    // planet labels
    if (showLabels) {
        for (int i = 0; i < planetCount; i++) {
            float x = planets[i].orbitRadius * cos(planets[i].orbitAngle * 3.14159f / 180.0f);
            float z = planets[i].orbitRadius * sin(planets[i].orbitAngle * 3.14159f / 180.0f);
            float y = planets[i].radius + 0.3f;

            // project 3D → 2D
            GLdouble mx, my, mz;
            GLdouble model[16], proj[16];
            GLint    vp[4];
            glGetDoublev(GL_MODELVIEW_MATRIX,  model);
            glGetDoublev(GL_PROJECTION_MATRIX, proj);
            glGetIntegerv(GL_VIEWPORT,         vp);
            gluProject(x, y, z, model, proj, vp, &mx, &my, &mz);

            if (mz < 1.0) {
                glMatrixMode(GL_PROJECTION);
                glPushMatrix(); glLoadIdentity();
                gluOrtho2D(0, winW, 0, winH);
                glMatrixMode(GL_MODELVIEW);
                glPushMatrix(); glLoadIdentity();
                glDisable(GL_LIGHTING); glDisable(GL_DEPTH_TEST);
                glColor3f(1.0f, 1.0f, 0.6f);
                drawText((float)mx - 10, (float)my + 5, planets[i].name.c_str());
                glEnable(GL_DEPTH_TEST); glEnable(GL_LIGHTING);
                glMatrixMode(GL_PROJECTION); glPopMatrix();
                glMatrixMode(GL_MODELVIEW);  glPopMatrix();
            }
        }
    }

    drawHUD();
    if (selectedPlanet >= 0) drawInfoPanel(selectedPlanet);

    glutSwapBuffers();
}

// ── reshape ──────────────────────────────────────────────────────────────────
void reshape(int w, int h) {
    winW = w; winH = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / h, 0.1, 500.0);
    glMatrixMode(GL_MODELVIEW);
}

// ── keyboard ─────────────────────────────────────────────────────────────────
void keyboard(unsigned char key, int, int) {
    switch (key) {
        case ' ':  paused = !paused;                                    break;
        case '+': case '=': speedMult = (speedMult < 20.0f) ? speedMult + 0.25f : speedMult; break;
        case '-':             speedMult = (speedMult > 0.1f)  ? speedMult - 0.25f : speedMult; break;
        case 'o': case 'O': showOrbits = !showOrbits;                   break;
        case 'l': case 'L': showLabels = !showLabels;                   break;
        case 'r': case 'R': resetCamera(); break;
        case 'w': case 'W': camera.targetDistance = (camera.targetDistance > 10.0f)   ? camera.targetDistance - 1.5f : camera.targetDistance; break;
        case 's': case 'S': camera.targetDistance = (camera.targetDistance < 180.0f)  ? camera.targetDistance + 1.5f : camera.targetDistance; break;
        case 27:   exit(0);                                             break;
    }
    glutPostRedisplay();
}

// ── mouse click (planet selection + double-click focus) ──────────────────────
static int  lastClickTime   = 0;
static int  lastClickPlanet = -1;

void mouseClick(int button, int state, int x, int y) {
    cameraMouseButton(button, state, x, y);

    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        GLdouble model[16], proj[16];
        GLint    vp[4];
        glGetDoublev(GL_MODELVIEW_MATRIX,  model);
        glGetDoublev(GL_PROJECTION_MATRIX, proj);
        glGetIntegerv(GL_VIEWPORT,         vp);

        int best = -1;
        double bestDist = 30.0;
        for (int i = 0; i < planetCount; i++) {
            float px = planets[i].orbitRadius * cos(planets[i].orbitAngle * 3.14159f / 180.0f);
            float pz = planets[i].orbitRadius * sin(planets[i].orbitAngle * 3.14159f / 180.0f);
            GLdouble sx, sy, sz;
            gluProject(px, 0.0, pz, model, proj, vp, &sx, &sy, &sz);
            double dx = sx - x, dy = sy - (winH - y);
            double d  = sqrt(dx*dx + dy*dy);
            if (d < bestDist) { bestDist = d; best = i; }
        }
        selectedPlanet = best;

        // double-click detection (within 400ms on same planet)
        int now = glutGet(GLUT_ELAPSED_TIME);
        if (best >= 0 && best == lastClickPlanet && (now - lastClickTime) < 400) {
            float px = planets[best].orbitRadius * cos(planets[best].orbitAngle * 3.14159f / 180.0f);
            float pz = planets[best].orbitRadius * sin(planets[best].orbitAngle * 3.14159f / 180.0f);
            focusPlanet(px, pz, planets[best].radius);
        }
        lastClickTime   = now;
        lastClickPlanet = best;
    }
}

// ── arrow keys (navigate between planets) ───────────────────────────────────
void specialKeys(int key, int, int) {
    if (key == GLUT_KEY_RIGHT)
        selectedPlanet = (selectedPlanet + 1 + planetCount) % planetCount;
    else if (key == GLUT_KEY_LEFT)
        selectedPlanet = (selectedPlanet - 1 + planetCount) % planetCount;

    if (selectedPlanet >= 0) {
        float px = planets[selectedPlanet].orbitRadius * cos(planets[selectedPlanet].orbitAngle * 3.14159f / 180.0f);
        float pz = planets[selectedPlanet].orbitRadius * sin(planets[selectedPlanet].orbitAngle * 3.14159f / 180.0f);
        focusPlanet(px, pz, planets[selectedPlanet].radius);
    }
    glutPostRedisplay();
}

// ── main ─────────────────────────────────────────────────────────────────────
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(winW, winH);
    glutCreateWindow("3D Solar System Simulation");

    glClearColor(0.0f, 0.0f, 0.02f, 1.0f);

    initPlanets();
    initCamera();
    initLighting();
    initRenderer();
    loadAllTextures();
    initAudio();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutMouseFunc(mouseClick);
    glutMotionFunc(cameraMouseMotion);
    glutTimerFunc(16, update, 0);

    glutMainLoop();
    cleanupAudio();
    return 0;
}
