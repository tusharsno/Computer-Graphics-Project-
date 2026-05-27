#include "planet.h"

Planet planets[8];
int planetCount = 8;

void initPlanets() {
    // name, radius, orbitRadius, orbitSpeed, rotationSpeed, orbitAngle, rotationAngle, textureID, r, g, b, hasMoon, hasRing, moonAngle, moonOrbitRadius, moonOrbitSpeed
    planets[0] = {"Mercury", 0.4f,  4.0f,  4.74f, 0.017f, 0.0f, 0.0f, 0, 0.7f, 0.7f, 0.7f, false, false, 0.0f, 0.0f, 0.0f};
    planets[1] = {"Venus",   0.9f,  7.0f,  1.86f, 0.004f, 0.0f, 0.0f, 0, 0.9f, 0.7f, 0.4f, false, false, 0.0f, 0.0f, 0.0f};
    planets[2] = {"Earth",   1.0f,  10.0f, 1.00f, 1.000f, 0.0f, 0.0f, 0, 0.2f, 0.5f, 1.0f, true,  false, 0.0f, 1.8f, 3.0f};
    planets[3] = {"Mars",    0.5f,  14.0f, 0.53f, 0.970f, 0.0f, 0.0f, 0, 0.8f, 0.3f, 0.2f, false, false, 0.0f, 0.0f, 0.0f};
    planets[4] = {"Jupiter", 2.5f,  20.0f, 0.08f, 2.400f, 0.0f, 0.0f, 0, 0.8f, 0.6f, 0.4f, false, false, 0.0f, 0.0f, 0.0f};
    planets[5] = {"Saturn",  2.0f,  27.0f, 0.03f, 2.200f, 0.0f, 0.0f, 0, 0.9f, 0.8f, 0.5f, false, true,  0.0f, 0.0f, 0.0f};
    planets[6] = {"Uranus",  1.5f,  33.0f, 0.01f, 1.400f, 0.0f, 0.0f, 0, 0.5f, 0.8f, 0.9f, false, false, 0.0f, 0.0f, 0.0f};
    planets[7] = {"Neptune", 1.4f,  38.0f, 0.006f,1.500f, 0.0f, 0.0f, 0, 0.2f, 0.3f, 0.9f, false, false, 0.0f, 0.0f, 0.0f};
}
