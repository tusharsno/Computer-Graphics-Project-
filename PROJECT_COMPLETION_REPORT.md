# 3D Solar System Simulation
## Project Completion Report (Final)

**Course:** Computer Graphics and Image Processing
**Department:** Computer Science and Engineering (CSE)
**University:** USTC
**Semester:** 7th Semester
**Team Size:** 2 Members
**Technology:** C++ | OpenGL (Fixed Pipeline) | GLUT | OpenAL
**Status:** ✅ FULLY COMPLETED

---

## Table of Contents

1. [Project Summary](#1-project-summary)
2. [Complete Features List](#2-complete-features-list)
3. [CG Topics Covered](#3-cg-topics-covered)
4. [Final Folder Structure](#4-final-folder-structure)
5. [Module Implementation Details](#5-module-implementation-details)
6. [Planet Data Reference](#6-planet-data-reference)
7. [Controls Reference](#7-controls-reference)
8. [How to Build & Run](#8-how-to-build--run)
9. [Known Limitations](#9-known-limitations)
10. [References](#10-references)

---

## 1. Project Summary

A real-time 3D interactive Solar System simulation built from scratch using C++ and OpenGL. The simulation renders the Sun, all 9 planets (including Pluto), multiple moons, Saturn's ring, Uranus's ring, asteroid belt, Halley's Comet, and a Milky Way background — all with real textures, Phong lighting, orbital mechanics, and full user interaction.

**No game engine. No external framework. Pure OpenGL + C++.**

---

## 2. Complete Features List

### Celestial Bodies
| Object | Details |
|--------|---------|
| ☀️ Sun | Procedural fiery glow, animated surface layers, additive bloom |
| 🪐 Mercury | Real texture, axial tilt 0.03°, orbital inclination 7.0° |
| 🪐 Venus | Real texture, retrograde rotation (177.4°), inclination 3.4° |
| 🌍 Earth | Real texture, cloud layer, night city lights, axial tilt 23.5° |
| 🔴 Mars | Real texture, axial tilt 25.2°, inclination 1.9° |
| 🟠 Jupiter | Real texture, largest planet, 3 moons (Io, Europa, Ganymede) |
| 🪐 Saturn | Real texture, wide ring with alpha blending |
| 🔵 Uranus | Real texture, extreme tilt 97.8°, thin bluish ring |
| 🔵 Neptune | Real texture, axial tilt 28.3° |
| ⚪ Pluto | Dwarf planet, extreme inclination 17.1° |
| 🌙 Earth's Moon | Real texture, orbits Earth |
| 🔴 Mars Moons | Phobos & Deimos orbiting Mars |
| ☄️ Halley's Comet | Elliptical orbit, Kepler speed, animated tail |
| ☄️ Asteroid Belt | 300 random asteroids between Mars & Jupiter |

### Visual Effects
| Effect | Implementation |
|--------|---------------|
| Sun Glow | Multi-layer additive blend spheres |
| Atmosphere Halo | Earth (blue), Venus (yellow), Mars (red) |
| Earth Clouds | Semi-transparent rotating cloud layer |
| Earth Night Lights | City lights on dark side (additive blend) |
| Saturn Ring | Triangle strip with alpha gradient |
| Uranus Ring | Thin bluish ring matching axial tilt |
| Shooting Stars | Random spawning with fading tail |
| Milky Way Background | Real texture on inverted sphere |
| Star Field | 1500 randomly placed point stars |

### Interaction & Controls
| Feature | Implementation |
|---------|---------------|
| Camera Rotation | Mouse drag (spherical coordinates) |
| Zoom | Scroll wheel + W/S keys (smooth lerp) |
| Planet Focus | Double-click → smooth camera transition |
| Arrow Navigation | ← → keys cycle through planets |
| Telescope Mode | T key → FOV 45° → 10° smooth transition |
| Pause/Resume | SPACE key |
| Speed Control | = / - keys (0.1x to 20x) |
| Orbit Toggle | O key |
| Label Toggle | L key |
| Camera Reset | R key |
| Screenshot | F1 key → saves .ppm file |

### Audio
| Feature | Implementation |
|---------|---------------|
| Space Ambient | Procedurally generated using OpenAL |
| Sound Design | Layered sine waves (55Hz base + harmonics) |
| LFO Modulation | Slow breathing/pulsing effect |
| Loop | 60-second seamless loop |

---

## 3. CG Topics Covered

| CG Topic | Implementation |
|----------|---------------|
| 3D Transformations | Planet orbit, self-rotation, axial tilt, orbital inclination |
| Perspective Projection | `gluPerspective` with dynamic FOV (telescope mode) |
| Orthographic Projection | 2D HUD overlay using `gluOrtho2D` |
| Viewing / Camera | `gluLookAt` with spherical coordinates + smooth lerp |
| Lighting Model | Sun as `GL_LIGHT0` point light source |
| Phong Shading | Ambient + Diffuse + Specular + Shininess per planet |
| Texture Mapping | Real planet textures via `stb_image.h` |
| Alpha Blending | Rings, atmosphere, glow, clouds, night lights |
| Animation | Timer-based 60 FPS loop with physics update |
| Z-Buffering | `GL_DEPTH_TEST` + `glDepthMask` for glow layers |
| Geometric Primitives | Sphere, Line Loop, Triangle Strip, Points, Lines |
| Coordinate Systems | World → Object → Camera space pipeline |
| Kepler's Law | Comet speed varies with distance from Sun |
| Particle System | Asteroid belt, shooting stars |

---

## 4. Final Folder Structure

```
Graphics_Project/
│
├── main.cpp                  # Entry point, game loop, all input handling
│
├── src/
│   ├── planet.cpp / .h       # Planet struct with 18 properties each
│   ├── renderer.cpp / .h     # All rendering (sun, planets, comet, etc.)
│   ├── camera.cpp / .h       # Smooth camera with focus & lerp
│   ├── lighting.cpp / .h     # OpenGL lighting setup
│   ├── texture.cpp / .h      # stb_image texture loader
│   ├── ui.cpp / .h           # HUD, labels, info panel, speed indicator
│   └── audio.cpp / .h        # Procedural space ambient (OpenAL)
│
├── include/
│   └── stb_image.h           # Header-only image loader
│
├── textures/
│   ├── sun.jpg               # Sun surface
│   ├── mercury.jpg           # Mercury surface
│   ├── venus.jpg             # Venus surface
│   ├── earth.jpg             # Earth day map
│   ├── earth_clouds.jpg      # Earth cloud layer
│   ├── earth_night.jpg       # Earth night city lights
│   ├── mars.jpg              # Mars surface
│   ├── jupiter.jpg           # Jupiter surface
│   ├── saturn.jpg            # Saturn surface
│   ├── uranus.jpg            # Uranus surface
│   ├── neptune.jpg           # Neptune surface
│   ├── moon.jpg              # Earth's Moon
│   └── milkyway.jpg          # Background star field
│
├── Makefile
├── solar_system              # Compiled executable
├── README.md
├── PROJECT_DOCUMENTATION.md  # Initial planning document
└── PROJECT_COMPLETION_REPORT.md  # This file
```

---

## 5. Module Implementation Details

### planet.cpp / planet.h
Each planet stores 18 properties:
```cpp
struct Planet {
    string name;
    float radius, orbitRadius;
    float orbitSpeed, rotationSpeed;
    float orbitAngle, rotationAngle;
    GLuint textureID;
    float r, g, b;
    bool hasMoon, hasRing;
    float moonAngle, moonOrbitRadius, moonOrbitSpeed;
    float axialTilt;
    float orbitInclination;
};
```

### renderer.cpp
- `drawSun()` — solid core + 3 animated fiery layers + 4 glow spheres (additive blend)
- `drawPlanet(idx)` — textured sphere + axial tilt + inclination + night lights + clouds
- `drawAtmosphere(x,y,z,r,g,b)` — 3-layer halo for Earth, Venus, Mars
- `drawMoon(idx)` — textured moon orbiting Earth
- `drawComet()` — elliptical orbit with Kepler speed + animated tail
- `drawSaturnRing()` — triangle strip with alpha gradient
- `drawAsteroidBelt()` — 300 random small spheres
- `drawShootingStars()` — 5 concurrent shooters with fading tails
- `drawStars()` — milky way texture sphere + 1500 point stars

### camera.cpp
- Spherical coordinate system (angleX, angleY, distance)
- Smooth lerp for all transitions (zoom, focus, angle)
- `focusPlanet(px, py, pz, radius)` — smooth camera move to planet
- Mouse drag instantly updates angles (no lag)
- Scroll + W/S for zoom with target lerp

### audio.cpp
- OpenAL device + context initialization
- Procedural 60-second PCM buffer generation
- Layered sine waves: 55Hz, 82.5Hz, 110Hz, 165Hz, 220Hz
- Two LFO modulators (0.05Hz, 0.03Hz) for breathing effect
- Seamless loop with fade in/out at endpoints

### ui.cpp
- Speed indicator (green/orange/red based on speed)
- Pause indicator (red `[ PAUSED ]`)
- Telescope mode indicator (cyan `[ TELESCOPE MODE ]`)
- Planet info panel (name, type, distance AU, orbital period)
- Planet labels projected from 3D to 2D using `gluProject`

---

## 6. Planet Data Reference

| Planet | Radius | Orbit | Incl. | Tilt | Moons | Ring |
|--------|--------|-------|-------|------|-------|------|
| Mercury | 0.35 | 5.0 | 7.0° | 0.03° | — | — |
| Venus | 0.87 | 7.0 | 3.4° | 177.4° | — | — |
| Earth | 0.92 | 10.0 | 0.0° | 23.5° | Moon | — |
| Mars | 0.50 | 14.0 | 1.9° | 25.2° | Phobos, Deimos | — |
| Jupiter | 2.80 | 21.0 | 1.3° | 3.1° | Io, Europa, Ganymede | — |
| Saturn | 2.30 | 28.5 | 2.5° | 26.7° | — | Wide |
| Uranus | 1.40 | 35.0 | 0.8° | 97.8° | — | Thin |
| Neptune | 1.35 | 40.0 | 1.8° | 28.3° | — | — |
| Pluto | 0.18 | 46.0 | 17.1° | 122.5° | — | — |

---

## 7. Controls Reference

| Key / Action | Function |
|-------------|---------|
| Mouse Left Drag | Rotate camera |
| Scroll Up / W | Zoom In |
| Scroll Down / S | Zoom Out |
| SPACE | Pause / Resume |
| = | Speed Up |
| - | Speed Down |
| T | Toggle Telescope Mode |
| O | Toggle Orbit Lines |
| L | Toggle Planet Labels |
| R | Reset Camera |
| ← → Arrow Keys | Navigate between planets |
| Double-Click Planet | Focus camera on planet |
| F1 | Save Screenshot (.ppm) |
| ESC | Exit |

---

## 8. How to Build & Run

### Dependencies
```bash
sudo apt-get install freeglut3-dev libgl1-mesa-dev libglu1-mesa-dev libopenal-dev
```

### Build
```bash
cd Graphics_Project
make
```

### Run
```bash
./solar_system
```

### Clean
```bash
make clean
```

---

## 9. Known Limitations

- Uses OpenGL Legacy (Fixed-Function Pipeline) — intentional for course compatibility
- Planet sizes not to true astronomical scale — adjusted for visual clarity
- No real shadow casting between planets
- Halley's Comet orbit is simplified (not true 75-year period)
- Screenshot saves as .ppm format (open with GIMP or convert with ImageMagick)

---

## 10. References

1. OpenGL Programming Guide (Red Book) — 8th Edition
2. Computer Graphics with OpenGL — Donald Hearn & M. Pauline Baker
3. learnopengl.com — OpenGL tutorials
4. NASA Solar System Exploration — https://solarsystem.nasa.gov
5. Planet Textures — https://www.solarsystemscope.com/textures/
6. stb_image.h — https://github.com/nothings/stb
7. OpenAL Programmer's Guide — https://openal.org/documentation/

---

## Final Summary

> This project implements a **complete, scientifically inspired, visually rich 3D Solar System simulation** covering every major topic of the Computer Graphics course. It features 9 planets with real textures, multiple moons, rings, a procedurally generated space ambient soundtrack, Halley's Comet with Kepler's law, Earth's night city lights, atmosphere glow effects, telescope mode, screenshot capability, and full interactive camera control — all built from scratch using C++ and OpenGL without any game engine or external framework.

---

*Final Completion Report — CSE Department, 7th Semester, USTC*
*Course: Computer Graphics and Image Processing*
