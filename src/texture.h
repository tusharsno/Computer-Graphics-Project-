#pragma once
#include <GL/gl.h>
#include <string>

extern GLuint sunTextureID;
extern GLuint moonTextureID;
extern GLuint cloudTextureID;
extern GLuint nightTextureID;

GLuint loadTexture(const std::string& path);
void loadAllTextures();
