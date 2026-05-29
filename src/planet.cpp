#include "planet.h"

Planet planets[9];
int planetCount = 9;

void initPlanets() {
    // name, radius, orbitRadius, orbitSpeed, rotationSpeed, orbitAngle, rotationAngle, textureID, r, g, b, hasMoon, hasRing, moonAngle, moonOrbitRadius, moonOrbitSpeed, axialTilt, orbitInclination
    planets[0] = {"Mercury", 0.35f, 5.0f,  0.80f,  0.20f,   0.0f, 0.0f, 0, 0.7f, 0.7f, 0.7f, false, false, 0.0f, 0.0f, 0.0f,   0.03f,  7.0f};
    planets[1] = {"Venus",   0.87f, 7.0f,  0.31f,  0.15f,  45.0f, 0.0f, 0, 0.9f, 0.7f, 0.4f, false, false, 0.0f, 0.0f, 0.0f, 177.4f,  3.4f};
    planets[2] = {"Earth",   0.92f, 10.0f, 0.19f,  0.50f,  90.0f, 0.0f, 0, 0.2f, 0.5f, 1.0f, true,  false, 0.0f, 1.8f, 3.0f,  23.5f,  0.0f};
    planets[3] = {"Mars",    0.50f, 14.0f, 0.10f,  0.49f, 135.0f, 0.0f, 0, 0.8f, 0.3f, 0.2f, false, false, 0.0f, 0.0f, 0.0f,  25.2f,  1.9f};
    planets[4] = {"Jupiter", 2.80f, 21.0f, 0.016f, 1.20f, 200.0f, 0.0f, 0, 0.8f, 0.6f, 0.4f, false, false, 0.0f, 0.0f, 0.0f,   3.1f,  1.3f};
    planets[5] = {"Saturn",  2.30f, 28.5f, 0.007f, 1.10f, 260.0f, 0.0f, 0, 0.9f, 0.8f, 0.5f, false, true,  0.0f, 0.0f, 0.0f,  26.7f,  2.5f};
    planets[6] = {"Uranus",  1.40f, 35.0f, 0.0025f,0.70f, 310.0f, 0.0f, 0, 0.5f, 0.8f, 0.9f, false, true,  0.0f, 0.0f, 0.0f,  97.8f,  0.8f};
    planets[7] = {"Neptune", 1.35f, 40.0f, 0.0015f,0.75f,   5.0f, 0.0f, 0, 0.2f, 0.3f, 0.9f, false, false, 0.0f, 0.0f, 0.0f,  28.3f,  1.8f};
    planets[8] = {"Pluto",   0.18f, 46.0f, 0.001f, 0.08f,  55.0f, 0.0f, 0, 0.8f, 0.7f, 0.6f, false, false, 0.0f, 0.0f, 0.0f, 122.5f, 17.1f};
}
