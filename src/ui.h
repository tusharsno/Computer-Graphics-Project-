#pragma once

extern bool showLabels;
extern bool showOrbits;
extern int  selectedPlanet;

void drawText(float x, float y, const char* text);
void drawHUD();
void drawPlanetLabel(float wx, float wy, float wz, const char* name);
void drawInfoPanel(int planetIndex);
