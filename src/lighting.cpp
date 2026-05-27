#include "lighting.h"
#include <GL/gl.h>

void initLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);

    GLfloat ambient[]  = {0.18f, 0.18f, 0.18f, 1.0f};
    GLfloat diffuse[]  = {1.0f,  1.0f,  0.95f, 1.0f};
    GLfloat specular[] = {1.0f,  1.0f,  1.0f,  1.0f};
    GLfloat position[] = {0.0f,  0.0f,  0.0f,  1.0f};

    glLightfv(GL_LIGHT0, GL_AMBIENT,  ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
}

void updateLighting() {
    GLfloat position[] = {0.0f, 0.0f, 0.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, position);
}
