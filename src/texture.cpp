#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"
#include "texture.h"
#include "planet.h"
#include <GL/gl.h>
#include <iostream>

GLuint loadTexture(const std::string& path) {
    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    int w, h, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path.c_str(), &w, &h, &channels, 0);

    if (data) {
        GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        stbi_image_free(data);
    } else {
        std::cerr << "Failed to load texture: " << path << std::endl;
        glDeleteTextures(1, &texID);
        return 0;
    }
    return texID;
}

GLuint sunTextureID  = 0;
GLuint moonTextureID = 0;

void loadAllTextures() {
    const std::string names[] = {"mercury","venus","earth","mars","jupiter","saturn","uranus","neptune"};
    for (int i = 0; i < 8; i++)
        planets[i].textureID = loadTexture("textures/" + names[i] + ".jpg");
    sunTextureID  = loadTexture("textures/sun.jpg");
    moonTextureID = loadTexture("textures/moon.jpg");
}
