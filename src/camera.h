#pragma once

struct Camera {
    float angleX, angleY;
    float distance;
    float targetDistance;
    float targetAngleX, targetAngleY;
    float lookAtX, lookAtZ;         // current look-at point
    float targetLookAtX, targetLookAtZ;
    float lastMouseX, lastMouseY;
    bool dragging;
};

extern Camera camera;

void initCamera();
void cameraMouseButton(int button, int state, int x, int y);
void cameraMouseMotion(int x, int y);
void applyCamera();
void resetCamera();
void smoothUpdateCamera();
void focusPlanet(float px, float pz, float planetRadius);
