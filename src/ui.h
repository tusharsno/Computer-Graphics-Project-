#pragma once

extern bool  showLabels;
extern bool  showOrbits;
extern int   selectedPlanet;
extern float currentSpeed;
extern bool  isPaused;

void drawText(float x, float y, const char* text);
void drawHUD();
void drawInfoPanel(int planetIndex);
