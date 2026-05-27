#include "camera.h"
#include <GL/glu.h>
#include <GL/glut.h>
#include <cmath>

Camera camera;

void initCamera() {
    camera.angleX   = 30.0f;
    camera.angleY   = 0.0f;
    camera.distance = 55.0f;
    camera.dragging = false;
}

void resetCamera() {
    initCamera();
}

void applyCamera() {
    glLoadIdentity();
    float rad_x = camera.angleX * 3.14159f / 180.0f;
    float rad_y = camera.angleY * 3.14159f / 180.0f;
    float ex = camera.distance * cos(rad_x) * sin(rad_y);
    float ey = camera.distance * sin(rad_x);
    float ez = camera.distance * cos(rad_x) * cos(rad_y);
    gluLookAt(ex, ey, ez, 0, 0, 0, 0, 1, 0);
}

void cameraMouseButton(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        camera.dragging   = (state == GLUT_DOWN);
        camera.lastMouseX = x;
        camera.lastMouseY = y;
    }
    // scroll wheel
    if (button == 3) camera.distance = (camera.distance > 10.0f) ? camera.distance - 1.5f : camera.distance;
    if (button == 4) camera.distance = (camera.distance < 120.0f) ? camera.distance + 1.5f : camera.distance;
}

void cameraMouseMotion(int x, int y) {
    if (!camera.dragging) return;
    float dx = x - camera.lastMouseX;
    float dy = y - camera.lastMouseY;
    camera.angleY += dx * 0.4f;
    camera.angleX += dy * 0.4f;
    if (camera.angleX >  89.0f) camera.angleX =  89.0f;
    if (camera.angleX < -89.0f) camera.angleX = -89.0f;
    camera.lastMouseX = x;
    camera.lastMouseY = y;
    glutPostRedisplay();
}
