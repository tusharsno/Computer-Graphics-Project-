#pragma once

void initRenderer();
void renderScene();
void drawSun();
void drawPlanet(int index);
void drawOrbit(float radius, float inclination);
void drawStars();
void drawShootingStars();
void drawSaturnRing(float innerR, float outerR);
void drawAsteroidBelt();
void drawMoon(int planetIndex);
