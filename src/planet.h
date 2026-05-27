#pragma once
#include <string>
#include <GL/gl.h>

struct Planet {
    std::string name;
    float radius;
    float orbitRadius;
    float orbitSpeed;
    float rotationSpeed;
    float orbitAngle;
    float rotationAngle;
    GLuint textureID;
    float r, g, b;
    bool hasMoon;
    bool hasRing;
    float moonAngle;
    float moonOrbitRadius;
    float moonOrbitSpeed;
    float axialTilt;  // degrees
};

extern Planet planets[9];
extern int planetCount;

void initPlanets();
