#include "camera.h"
#include <GL/glu.h>
#include <GL/glut.h>
#include <cmath>

Camera camera;

void initCamera() {
    camera.angleX          = 30.0f;
    camera.angleY          = 0.0f;
    camera.distance        = 55.0f;
    camera.targetDistance  = 55.0f;
    camera.targetAngleX    = 30.0f;
    camera.targetAngleY    = 0.0f;
    camera.lookAtX         = 0.0f;
    camera.lookAtZ         = 0.0f;
    camera.targetLookAtX   = 0.0f;
    camera.targetLookAtZ   = 0.0f;
    camera.dragging        = false;
}

void resetCamera() {
    initCamera();
}

void focusPlanet(float px, float pz, float planetRadius) {
    camera.targetLookAtX  = px;
    camera.targetLookAtZ  = pz;
    camera.targetDistance = planetRadius * 5.0f + 3.0f;
    camera.targetAngleX   = 20.0f;
}

void smoothUpdateCamera() {
    float speed = 0.08f;
    camera.distance  += (camera.targetDistance  - camera.distance)  * speed;
    camera.angleX    += (camera.targetAngleX    - camera.angleX)    * speed;
    camera.angleY    += (camera.targetAngleY    - camera.angleY)    * speed;
    camera.lookAtX   += (camera.targetLookAtX   - camera.lookAtX)   * speed;
    camera.lookAtZ   += (camera.targetLookAtZ   - camera.lookAtZ)   * speed;
}

void applyCamera() {
    glLoadIdentity();
    float rad_x = camera.angleX * 3.14159f / 180.0f;
    float rad_y = camera.angleY * 3.14159f / 180.0f;
    float ex = camera.lookAtX + camera.distance * cos(rad_x) * sin(rad_y);
    float ey =                  camera.distance * sin(rad_x);
    float ez = camera.lookAtZ + camera.distance * cos(rad_x) * cos(rad_y);
    gluLookAt(ex, ey, ez, camera.lookAtX, 0, camera.lookAtZ, 0, 1, 0);
}

void cameraMouseButton(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        camera.dragging   = (state == GLUT_DOWN);
        camera.lastMouseX = x;
        camera.lastMouseY = y;
    }
    // scroll wheel
    if (button == 3) { camera.targetDistance = (camera.targetDistance > 10.0f)  ? camera.targetDistance - 1.5f : camera.targetDistance; }
    if (button == 4) { camera.targetDistance = (camera.targetDistance < 120.0f) ? camera.targetDistance + 1.5f : camera.targetDistance; }
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
