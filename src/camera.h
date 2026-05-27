#pragma once

struct Camera {
    float angleX, angleY;
    float distance;
    float lastMouseX, lastMouseY;
    bool dragging;
};

extern Camera camera;

void initCamera();
void cameraMouseButton(int button, int state, int x, int y);
void cameraMouseMotion(int x, int y);
void cameraScroll(int button, int dir, int x, int y);
void applyCamera();
void resetCamera();
