#include "planet.h"

Planet planets[9];
int planetCount = 9;

void initPlanets() {
    // name, radius, orbitRadius, orbitSpeed, rotationSpeed, orbitAngle, rotationAngle, textureID, r, g, b, hasMoon, hasRing, moonAngle, moonOrbitRadius, moonOrbitSpeed, axialTilt
    planets[0] = {"Mercury", 0.4f,  4.0f,  0.80f, 0.017f,  0.0f, 0.0f, 0, 0.7f, 0.7f, 0.7f, false, false,   0.0f, 0.0f, 0.0f,  0.03f};
    planets[1] = {"Venus",   0.9f,  7.0f,  0.31f, 0.004f,  45.0f, 0.0f, 0, 0.9f, 0.7f, 0.4f, false, false,  0.0f, 0.0f, 0.0f, 177.4f};
    planets[2] = {"Earth",   1.0f,  10.0f, 0.19f, 1.000f,  90.0f, 0.0f, 0, 0.2f, 0.5f, 1.0f, true,  false,  0.0f, 1.8f, 3.0f,  23.5f};
    planets[3] = {"Mars",    0.5f,  14.0f, 0.10f, 0.970f, 135.0f, 0.0f, 0, 0.8f, 0.3f, 0.2f, false, false,  0.0f, 0.0f, 0.0f,  25.2f};
    planets[4] = {"Jupiter", 2.5f,  20.0f, 0.016f,2.400f, 200.0f, 0.0f, 0, 0.8f, 0.6f, 0.4f, false, false,  0.0f, 0.0f, 0.0f,   3.1f};
    planets[5] = {"Saturn",  2.0f,  27.0f, 0.007f,2.200f, 260.0f, 0.0f, 0, 0.9f, 0.8f, 0.5f, false, true,   0.0f, 0.0f, 0.0f,  26.7f};
    planets[6] = {"Uranus",  1.5f,  33.0f, 0.0025f,1.400f,310.0f, 0.0f, 0, 0.5f, 0.8f, 0.9f, false, false,  0.0f, 0.0f, 0.0f,  97.8f};
    planets[7] = {"Neptune", 1.4f,  38.0f, 0.0015f,1.500f,  5.0f, 0.0f, 0, 0.2f, 0.3f, 0.9f, false, false,  0.0f, 0.0f, 0.0f,  28.3f};
    planets[8] = {"Pluto",   0.2f,  44.0f, 0.001f,0.156f,  55.0f, 0.0f, 0, 0.8f, 0.7f, 0.6f, false, false,  0.0f, 0.0f, 0.0f, 122.5f};
}
