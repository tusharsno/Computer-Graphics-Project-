#pragma once
#include <GL/gl.h>
#include <string>

extern GLuint sunTextureID;
extern GLuint moonTextureID;

GLuint loadTexture(const std::string& path);
void loadAllTextures();
