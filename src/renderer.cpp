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

static GLuint bgTextureID = 0;

// Halley's Comet
struct Comet {
    float angle;      // current position on orbit
    float speed;
    float a;          // semi-major axis
    float b;          // semi-minor axis
    float tailLen;
};
static Comet halley = {0.0f, 0.008f, 50.0f, 12.0f, 0.0f};

static void drawComet() {
    halley.angle += halley.speed;
    if (halley.angle > 360.0f) halley.angle -= 360.0f;

    float rad = halley.angle * 3.14159f / 180.0f;
    // elliptical position
    float cx = halley.a * cos(rad) - (halley.a - halley.b);
    float cz = halley.b * sin(rad);

    // distance from sun — closer = faster (Kepler)
    float dist = sqrt(cx*cx + cz*cz);
    halley.speed = 0.35f / (dist * 0.08f + 0.5f);

    // tail direction — away from sun
    float tx = cx / (dist + 0.001f);
    float tz = cz / (dist + 0.001f);
    float tailLen = 3.0f + 8.0f * (1.0f - dist / halley.a);

    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    // tail
    int tailSteps = 30;
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= tailSteps; i++) {
        float t     = (float)i / tailSteps;
        float alpha = (1.0f - t) * 0.6f;
        float width = (1.0f - t) * 0.12f;
        float px = cx + tx * t * tailLen;
        float pz = cz + tz * t * tailLen;
        glColor4f(0.7f, 0.9f, 1.0f, alpha);
        glVertex3f(px + tz * width, 0.0f, pz - tx * width);
        glVertex3f(px - tz * width, 0.0f, pz + tx * width);
    }
    glEnd();

    // nucleus
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
    glPushMatrix();
    glTranslatef(cx, 0.0f, cz);
    GLfloat ma[] = {0.5f,0.6f,0.7f,1.0f};
    GLfloat md[] = {0.7f,0.85f,1.0f,1.0f};
    GLfloat ms[] = {0.3f,0.3f,0.3f,1.0f};
    glMaterialfv(GL_FRONT, GL_AMBIENT,  ma);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,  md);
    glMaterialfv(GL_FRONT, GL_SPECULAR, ms);
    glMaterialf (GL_FRONT, GL_SHININESS, 10.0f);
    glDisable(GL_TEXTURE_2D);
    gluSphere(quad, 0.15f, 12, 12);
    glPopMatrix();
}

// shooting star state
struct ShootingStar {
    float x, y, z;       // current head position
    float dx, dy, dz;    // direction
    float life;          // 0..1
    float speed;
    bool  active;
};
static ShootingStar shooters[5];

static void spawnShooter(int i) {
    srand(glutGet(GLUT_ELAPSED_TIME) + i * 137);
    float theta = ((float)rand()/RAND_MAX) * 2.0f * 3.14159f;
    float phi   = ((float)rand()/RAND_MAX) * 3.14159f;
    float r     = 85.0f;
    shooters[i].x  = r * sin(phi) * cos(theta);
    shooters[i].y  = r * cos(phi);
    shooters[i].z  = r * sin(phi) * sin(theta);
    // direction: slightly inward + random
    shooters[i].dx = -shooters[i].x * 0.015f + ((float)rand()/RAND_MAX - 0.5f) * 0.3f;
    shooters[i].dy = -shooters[i].y * 0.015f + ((float)rand()/RAND_MAX - 0.5f) * 0.3f;
    shooters[i].dz = -shooters[i].z * 0.015f + ((float)rand()/RAND_MAX - 0.5f) * 0.3f;
    shooters[i].speed  = 0.8f + ((float)rand()/RAND_MAX) * 0.8f;
    shooters[i].life   = 0.0f;
    shooters[i].active = true;
}

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
    for (int i = 0; i < 5; i++) shooters[i].active = false;

    bgTextureID = loadTexture("textures/milkyway.jpg");
}

void drawShootingStars() {
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    static int spawnTimer = 0;
    spawnTimer++;

    // spawn a new shooter every ~180 frames (~3 sec)
    if (spawnTimer > 180) {
        spawnTimer = 0;
        for (int i = 0; i < 5; i++) {
            if (!shooters[i].active) { spawnShooter(i); break; }
        }
    }

    for (int i = 0; i < 5; i++) {
        if (!shooters[i].active) continue;

        shooters[i].x    += shooters[i].dx * shooters[i].speed;
        shooters[i].y    += shooters[i].dy * shooters[i].speed;
        shooters[i].z    += shooters[i].dz * shooters[i].speed;
        shooters[i].life += 0.018f;

        if (shooters[i].life >= 1.0f) { shooters[i].active = false; continue; }

        float alpha = (shooters[i].life < 0.2f)
                    ? shooters[i].life / 0.2f
                    : 1.0f - (shooters[i].life - 0.2f) / 0.8f;

        // tail: draw line from current pos back along direction
        float tailLen = 4.0f;
        float tx = shooters[i].x - shooters[i].dx * shooters[i].speed * tailLen;
        float ty = shooters[i].y - shooters[i].dy * shooters[i].speed * tailLen;
        float tz = shooters[i].z - shooters[i].dz * shooters[i].speed * tailLen;

        glLineWidth(1.5f);
        glBegin(GL_LINES);
            glColor4f(1.0f, 1.0f, 1.0f, alpha);
            glVertex3f(shooters[i].x, shooters[i].y, shooters[i].z);
            glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
            glVertex3f(tx, ty, tz);
        glEnd();
        glLineWidth(1.0f);
    }

    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
}

void drawStars() {
    // background sphere with milky way texture — drawn at camera position so it never clips
    if (bgTextureID) {
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, bgTextureID);
        glColor3f(1.0f, 1.0f, 1.0f);
        glPushMatrix();
        // translate to camera's look-at center so sphere always surrounds the view
        glScalef(-1.0f, 1.0f, 1.0f);
        gluSphere(quad, 200.0f, 32, 32);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
    }

    // point stars on top for extra depth
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

    static float sunRot = 0.0f;
    sunRot += 0.015f;

    // 1. solid bright core FIRST
    glColor3f(1.0f, 0.95f, 0.6f);
    gluSphere(quad, 2.0f, 64, 64);

    // 2. glow layers on top — disable depth write so they don't block each other
    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    // animated fiery surface
    glColor4f(1.0f, 0.35f, 0.0f, 0.45f);
    glPushMatrix(); glRotatef(sunRot * 40.0f, 0.3f, 1.0f, 0.2f);
    gluSphere(quad, 2.05f, 48, 48); glPopMatrix();

    glColor4f(1.0f, 0.15f, 0.0f, 0.30f);
    glPushMatrix(); glRotatef(-sunRot * 30.0f, 0.1f, 1.0f, 0.5f);
    gluSphere(quad, 2.07f, 48, 48); glPopMatrix();

    glColor4f(1.0f, 0.6f, 0.0f, 0.22f);
    glPushMatrix(); glRotatef(sunRot * 20.0f, 0.6f, 1.0f, 0.1f);
    gluSphere(quad, 2.09f, 48, 48); glPopMatrix();

    // outer glow
    glColor4f(1.0f, 0.65f, 0.15f, 0.18f); gluSphere(quad, 2.3f, 32, 32);
    glColor4f(1.0f, 0.6f, 0.1f, 0.12f);   gluSphere(quad, 2.5f, 32, 32);
    glColor4f(1.0f, 0.5f, 0.05f, 0.07f);  gluSphere(quad, 2.8f, 32, 32);
    glColor4f(1.0f, 0.4f, 0.0f, 0.04f);   gluSphere(quad, 3.2f, 32, 32);

    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);

    glEnable(GL_LIGHTING);
}

void drawOrbit(float radius, float inclination) {
    glDisable(GL_LIGHTING);
    glColor4f(0.4f, 0.4f, 0.6f, 0.5f);
    float incl = inclination * 3.14159f / 180.0f;
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 360; i++) {
        float rad = i * 3.14159f / 180.0f;
        float ox  = radius * cos(rad);
        float oy  = radius * sin(rad) * sin(incl);
        float oz  = radius * sin(rad) * cos(incl);
        glVertex3f(ox, oy, oz);
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
    glRotatef(p.moonAngle, 0.0f, 1.0f, 0.0f);

    GLfloat mat_amb[]  = {0.4f, 0.4f, 0.4f, 1.0f};
    GLfloat mat_diff[] = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat mat_spec[] = {0.1f, 0.1f, 0.1f, 1.0f};
    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_amb);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diff);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_spec);
    glMaterialf (GL_FRONT, GL_SHININESS, 5.0f);

    if (moonTextureID) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, moonTextureID);
    } else {
        glDisable(GL_TEXTURE_2D);
    }
    gluSphere(quad, 0.27f, 16, 16);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void drawAtmosphere(float x, float y, float z, float radius, float r, float g, float b) {
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_TEXTURE_2D);

    float layers[3][2] = {{0.06f, 0.18f}, {0.12f, 0.10f}, {0.20f, 0.05f}};
    for (int i = 0; i < 3; i++) {
        glPushMatrix();
        glTranslatef(x, y, z);
        glColor4f(r, g, b, layers[i][1]);
        gluSphere(quad, radius + layers[i][0], 32, 32);
        glPopMatrix();
    }

    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
}

void drawPlanet(int idx) {
    Planet& p = planets[idx];
    float rad    = p.orbitAngle * 3.14159f / 180.0f;
    float incl   = p.orbitInclination * 3.14159f / 180.0f;
    float x = p.orbitRadius * cos(rad);
    float y = p.orbitRadius * sin(rad) * sin(incl);
    float z = p.orbitRadius * sin(rad) * cos(incl);

    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(p.axialTilt, 0.0f, 0.0f, 1.0f);      // tilt the axis
    glRotatef(p.rotationAngle, 0.0f, 1.0f, 0.0f);  // spin around tilted axis

    GLfloat mat_amb[]  = {0.15f, 0.15f, 0.15f, 1.0f};
    GLfloat mat_diff[] = {1.0f,  1.0f,  1.0f,  1.0f};
    GLfloat mat_spec[] = {0.2f,  0.2f,  0.2f,  1.0f};
    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_amb);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diff);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_spec);
    glMaterialf (GL_FRONT, GL_SHININESS, 15.0f);

    if (p.textureID) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, p.textureID);
    } else {
        // no texture — use planet color
        GLfloat ca[] = {p.r*0.3f, p.g*0.3f, p.b*0.3f, 1.0f};
        GLfloat cd[] = {p.r,      p.g,      p.b,      1.0f};
        glMaterialfv(GL_FRONT, GL_AMBIENT,  ca);
        glMaterialfv(GL_FRONT, GL_DIFFUSE,  cd);
        glDisable(GL_TEXTURE_2D);
    }

    gluSphere(quad, p.radius, 48, 48);
    glDisable(GL_TEXTURE_2D);

    // Earth night side city lights (index 2)
    if (idx == 2 && nightTextureID) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);  // additive blend — lights add on top of dark side
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, nightTextureID);
        GLfloat na[] = {0.0f, 0.0f, 0.0f, 1.0f};
        GLfloat nd[] = {0.0f, 0.0f, 0.0f, 1.0f};  // diffuse=0 so only dark side shows lights
        GLfloat ne[] = {0.6f, 0.5f, 0.3f, 1.0f};  // emissive — city lights glow
        glMaterialfv(GL_FRONT, GL_AMBIENT,  na);
        glMaterialfv(GL_FRONT, GL_DIFFUSE,  nd);
        glMaterialfv(GL_FRONT, GL_EMISSION, ne);
        gluSphere(quad, p.radius + 0.001f, 48, 48);
        // reset emission
        GLfloat zero[] = {0.0f, 0.0f, 0.0f, 1.0f};
        glMaterialfv(GL_FRONT, GL_EMISSION, zero);
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_BLEND);
    }

    // Earth cloud layer (index 2)
    if (idx == 2 && cloudTextureID) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, cloudTextureID);
        GLfloat ca[] = {0.15f,0.15f,0.15f,1.0f};
        GLfloat cd[] = {1.0f, 1.0f, 1.0f, 0.45f};
        glMaterialfv(GL_FRONT, GL_AMBIENT, ca);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, cd);
        // rotate clouds slightly differently for realism
        glRotatef(p.rotationAngle * 0.3f, 0.0f, 1.0f, 0.0f);
        gluSphere(quad, p.radius + 0.02f, 48, 48);
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_BLEND);
    }

    // Saturn ring
    if (p.hasRing) {
        if (idx == 5) drawSaturnRing(p.radius * 1.3f, p.radius * 2.3f);
        else if (idx == 6) {
            // Uranus ring — thin, dark, bluish, tilted with planet (97.8 deg already applied)
            glDisable(GL_LIGHTING);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            int steps = 120;
            glBegin(GL_TRIANGLE_STRIP);
            for (int s = 0; s <= steps; s++) {
                float angle = s * 2.0f * 3.14159f / steps;
                float ca = cos(angle), sa = sin(angle);
                glColor4f(0.4f, 0.6f, 0.8f, 0.35f);
                glVertex3f((p.radius * 1.6f) * ca, 0.0f, (p.radius * 1.6f) * sa);
                glColor4f(0.3f, 0.5f, 0.7f, 0.15f);
                glVertex3f((p.radius * 1.3f) * ca, 0.0f, (p.radius * 1.3f) * sa);
            }
            glEnd();
            glDisable(GL_BLEND);
            glEnable(GL_LIGHTING);
        }
    }

    glPopMatrix();

    // Moon
    if (p.hasMoon) drawMoon(idx);

    // Mars moons: Phobos & Deimos (index 3)
    if (idx == 3) {
        static float marsMoonAngles[2] = {0.0f, 180.0f};
        static const float marsMoonSpeeds[2] = {5.0f, 2.0f};
        static const float marsMoonRadii[2]  = {0.9f, 1.5f};
        for (int m = 0; m < 2; m++) {
            marsMoonAngles[m] += marsMoonSpeeds[m] * 0.016f;
            float mx = x + marsMoonRadii[m] * cos(marsMoonAngles[m] * 3.14159f / 180.0f);
            float mz = z + marsMoonRadii[m] * sin(marsMoonAngles[m] * 3.14159f / 180.0f);
            glPushMatrix();
            glTranslatef(mx, 0.0f, mz);
            GLfloat ma[] = {0.35f,0.30f,0.25f,1.0f};
            GLfloat md[] = {0.55f,0.48f,0.40f,1.0f};
            GLfloat ms[] = {0.05f,0.05f,0.05f,1.0f};
            glMaterialfv(GL_FRONT, GL_AMBIENT,  ma);
            glMaterialfv(GL_FRONT, GL_DIFFUSE,  md);
            glMaterialfv(GL_FRONT, GL_SPECULAR, ms);
            glMaterialf (GL_FRONT, GL_SHININESS, 3.0f);
            glDisable(GL_TEXTURE_2D);
            gluSphere(quad, m == 0 ? 0.10f : 0.07f, 10, 10);
            glPopMatrix();
        }
    }

    // Jupiter's moons (index 4)
    if (idx == 4) {
        static float jupMoonAngles[3] = {0.0f, 120.0f, 240.0f};
        static const float jupMoonSpeeds[3]  = {4.5f, 2.2f, 1.1f};
        static const float jupMoonRadii[3]   = {3.2f, 4.2f, 5.5f};
        static const char* jupMoonNames[3]   = {"Io", "Europa", "Ganymede"};
        (void)jupMoonNames;

        for (int m = 0; m < 3; m++) {
            jupMoonAngles[m] += jupMoonSpeeds[m] * 0.016f;
            float jx = x + jupMoonRadii[m] * cos(jupMoonAngles[m] * 3.14159f / 180.0f);
            float jz = z + jupMoonRadii[m] * sin(jupMoonAngles[m] * 3.14159f / 180.0f);

            glPushMatrix();
            glTranslatef(jx, 0.0f, jz);
            GLfloat ma[] = {0.4f,0.4f,0.4f,1.0f};
            GLfloat md[] = {0.7f,0.7f,0.7f,1.0f};
            GLfloat ms[] = {0.1f,0.1f,0.1f,1.0f};
            glMaterialfv(GL_FRONT, GL_AMBIENT,  ma);
            glMaterialfv(GL_FRONT, GL_DIFFUSE,  md);
            glMaterialfv(GL_FRONT, GL_SPECULAR, ms);
            glMaterialf (GL_FRONT, GL_SHININESS, 5.0f);
            glDisable(GL_TEXTURE_2D);
            gluSphere(quad, 0.18f, 12, 12);
            glPopMatrix();
        }
    }
}

void renderScene() {
    drawStars();
    drawShootingStars();
    updateLighting();
    drawSun();

    if (showOrbits) {
        for (int i = 0; i < planetCount; i++)
            drawOrbit(planets[i].orbitRadius, planets[i].orbitInclination);
        // asteroid belt orbit hint
        glDisable(GL_LIGHTING);
        glColor4f(0.4f, 0.35f, 0.3f, 0.3f);
        glEnable(GL_LIGHTING);
    }

    drawAsteroidBelt();

    for (int i = 0; i < planetCount; i++)
        drawPlanet(i);

    // Halley's Comet
    drawComet();

    // comet orbit path
    if (showOrbits) {
        glDisable(GL_LIGHTING);
        glColor4f(0.4f, 0.6f, 0.8f, 0.25f);
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < 360; i++) {
            float r = i * 3.14159f / 180.0f;
            float ex = halley.a * cos(r) - (halley.a - halley.b);
            float ez = halley.b * sin(r);
            glVertex3f(ex, 0.0f, ez);
        }
        glEnd();
        glEnable(GL_LIGHTING);
    }

    // atmosphere glow — Earth (blue), Venus (yellow-white), Mars (red-orange)
    auto getPlanetPos = [](int i, float& px, float& py, float& pz) {
        float rad  = planets[i].orbitAngle * 3.14159f / 180.0f;
        float incl = planets[i].orbitInclination * 3.14159f / 180.0f;
        px = planets[i].orbitRadius * cos(rad);
        py = planets[i].orbitRadius * sin(rad) * sin(incl);
        pz = planets[i].orbitRadius * sin(rad) * cos(incl);
    };

    float ex, ey, ez, vx, vy, vz, mx, my, mz;
    getPlanetPos(2, ex, ey, ez); drawAtmosphere(ex, ey, ez, planets[2].radius, 0.3f, 0.6f, 1.0f);
    getPlanetPos(1, vx, vy, vz); drawAtmosphere(vx, vy, vz, planets[1].radius, 0.9f, 0.8f, 0.5f);
    getPlanetPos(3, mx, my, mz); drawAtmosphere(mx, my, mz, planets[3].radius, 0.9f, 0.4f, 0.2f);
}
