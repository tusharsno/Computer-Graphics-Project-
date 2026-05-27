#include "renderer.h"
#include "planet.h"
#include "ui.h"
#include "lighting.h"
#include "texture.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cmath>
#include <cstdlib>

static GLUquadric* quad = nullptr;

// pre-generated star positions
static float starX[1500], starY[1500], starZ[1500];

void initRenderer() {
    quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);
    gluQuadricNormals(quad, GLU_SMOOTH);

    srand(42);
    for (int i = 0; i < 1500; i++) {
        float theta = ((float)rand() / RAND_MAX) * 2.0f * 3.14159f;
        float phi   = ((float)rand() / RAND_MAX) * 3.14159f;
        float r     = 90.0f + ((float)rand() / RAND_MAX) * 10.0f;
        starX[i] = r * sin(phi) * cos(theta);
        starY[i] = r * cos(phi);
        starZ[i] = r * sin(phi) * sin(theta);
    }
}

void drawStars() {
    glDisable(GL_LIGHTING);
    glPointSize(1.5f);
    glBegin(GL_POINTS);
    for (int i = 0; i < 1500; i++) {
        float bright = 0.6f + 0.4f * ((float)rand() / RAND_MAX);
        glColor3f(bright, bright, bright);
        glVertex3f(starX[i], starY[i], starZ[i]);
    }
    glEnd();
    glPointSize(1.0f);
    glEnable(GL_LIGHTING);
}

void drawSun() {
    glDisable(GL_LIGHTING);

    // outer glow layers
    float glowColors[][4] = {
        {1.0f, 0.6f, 0.0f, 0.08f},
        {1.0f, 0.7f, 0.1f, 0.12f},
        {1.0f, 0.8f, 0.2f, 0.18f},
    };
    float glowSizes[] = {2.8f, 2.4f, 2.1f};

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    for (int g = 0; g < 3; g++) {
        glColor4fv(glowColors[g]);
        gluSphere(quad, glowSizes[g], 32, 32);
    }
    glDisable(GL_BLEND);

    glColor3f(1.0f, 0.95f, 0.4f);
    gluSphere(quad, 1.8f, 48, 48);

    glEnable(GL_LIGHTING);
}

void drawOrbit(float radius) {
    glDisable(GL_LIGHTING);
    glColor4f(0.4f, 0.4f, 0.6f, 0.5f);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 360; i++) {
        float rad = i * 3.14159f / 180.0f;
        glVertex3f(radius * cos(rad), 0.0f, radius * sin(rad));
    }
    glEnd();
    glEnable(GL_LIGHTING);
}

void drawSaturnRing(float innerR, float outerR) {
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    int steps = 120;
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= steps; i++) {
        float angle = i * 2.0f * 3.14159f / steps;
        float ca = cos(angle), sa = sin(angle);
        float t = (float)i / steps;
        float alpha = 0.55f - 0.2f * fabs(t - 0.5f) * 2.0f;
        glColor4f(0.85f, 0.75f, 0.55f, alpha);
        glVertex3f(outerR * ca, 0.0f, outerR * sa);
        glColor4f(0.75f, 0.65f, 0.45f, alpha * 0.7f);
        glVertex3f(innerR * ca, 0.0f, innerR * sa);
    }
    glEnd();

    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
}

void drawAsteroidBelt() {
    glDisable(GL_LIGHTING);
    glColor3f(0.55f, 0.50f, 0.45f);
    srand(123);
    for (int i = 0; i < 300; i++) {
        float angle  = ((float)rand() / RAND_MAX) * 2.0f * 3.14159f;
        float radius = 16.5f + ((float)rand() / RAND_MAX) * 1.8f;
        float yOff   = ((float)rand() / RAND_MAX) * 0.4f - 0.2f;
        float x = radius * cos(angle);
        float z = radius * sin(angle);
        glPushMatrix();
        glTranslatef(x, yOff, z);
        glutSolidSphere(0.04f + ((float)rand() / RAND_MAX) * 0.06f, 4, 4);
        glPopMatrix();
    }
    glEnable(GL_LIGHTING);
}

void drawMoon(int idx) {
    Planet& p = planets[idx];
    float mx = p.orbitRadius * cos(p.orbitAngle * 3.14159f / 180.0f)
             + p.moonOrbitRadius * cos(p.moonAngle * 3.14159f / 180.0f);
    float mz = p.orbitRadius * sin(p.orbitAngle * 3.14159f / 180.0f)
             + p.moonOrbitRadius * sin(p.moonAngle * 3.14159f / 180.0f);

    glPushMatrix();
    glTranslatef(mx, 0.0f, mz);

    GLfloat mat_amb[]  = {0.5f, 0.5f, 0.5f, 1.0f};
    GLfloat mat_diff[] = {0.7f, 0.7f, 0.7f, 1.0f};
    GLfloat mat_spec[] = {0.1f, 0.1f, 0.1f, 1.0f};
    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_amb);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diff);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_spec);
    glMaterialf (GL_FRONT, GL_SHININESS, 5.0f);

    glDisable(GL_TEXTURE_2D);
    gluSphere(quad, 0.27f, 16, 16);
    glPopMatrix();
}

void drawPlanet(int idx) {
    Planet& p = planets[idx];
    float x = p.orbitRadius * cos(p.orbitAngle * 3.14159f / 180.0f);
    float z = p.orbitRadius * sin(p.orbitAngle * 3.14159f / 180.0f);

    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    glRotatef(p.rotationAngle, 0.0f, 1.0f, 0.0f);

    GLfloat mat_amb[]  = {p.r * 0.3f, p.g * 0.3f, p.b * 0.3f, 1.0f};
    GLfloat mat_diff[] = {p.r,        p.g,        p.b,        1.0f};
    GLfloat mat_spec[] = {0.3f, 0.3f, 0.3f, 1.0f};
    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_amb);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diff);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_spec);
    glMaterialf (GL_FRONT, GL_SHININESS, 20.0f);

    if (p.textureID) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, p.textureID);
    } else {
        glDisable(GL_TEXTURE_2D);
    }

    gluSphere(quad, p.radius, 48, 48);
    glDisable(GL_TEXTURE_2D);

    // Saturn ring
    if (p.hasRing) {
        drawSaturnRing(p.radius * 1.3f, p.radius * 2.3f);
    }

    glPopMatrix();

    // Moon
    if (p.hasMoon) drawMoon(idx);
}

void renderScene() {
    drawStars();
    updateLighting();
    drawSun();

    if (showOrbits) {
        for (int i = 0; i < planetCount; i++)
            drawOrbit(planets[i].orbitRadius);
        // asteroid belt orbit hint
        glDisable(GL_LIGHTING);
        glColor4f(0.4f, 0.35f, 0.3f, 0.3f);
        glEnable(GL_LIGHTING);
    }

    drawAsteroidBelt();

    for (int i = 0; i < planetCount; i++)
        drawPlanet(i);
}
