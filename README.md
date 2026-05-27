# 🌍 3D Solar System Simulation

A real-time interactive 3D Solar System simulation built with **C++ and OpenGL**.

![OpenGL](https://img.shields.io/badge/OpenGL-GLUT-blue) ![C++](https://img.shields.io/badge/C++-17-brightgreen) ![Platform](https://img.shields.io/badge/Platform-Linux-orange)

---

## 📸 Features

- ☀️ Glowing Sun at the center as a point light source
- 🪐 All 8 planets with **real surface textures**
- 🌙 Earth's Moon orbiting Earth
- 💍 Saturn's ring with alpha blending
- ☄️ Asteroid belt between Mars and Jupiter
- ⭐ 1500 randomly placed background stars
- 🎮 Fully interactive camera (rotate, zoom)
- ⏸️ Pause/Resume, speed control
- 🖱️ Click on a planet to see its info
- 🔭 Toggle orbit lines and planet labels

---

## 🎮 Controls

| Key / Action | Function |
|-------------|---------|
| Mouse Left Drag | Rotate camera |
| Scroll Up / `W` | Zoom In |
| Scroll Down / `S` | Zoom Out |
| `SPACE` | Pause / Resume |
| `+` / `-` | Speed Up / Slow Down |
| `O` | Toggle orbit lines |
| `L` | Toggle planet labels |
| `R` | Reset camera |
| Left Click on Planet | Show planet info |
| `ESC` | Exit |

---

## 🛠️ Build & Run

### Install Dependencies
```bash
sudo apt-get install freeglut3-dev libgl1-mesa-dev libglu1-mesa-dev
```

### Build
```bash
make
```

### Run
```bash
./solar_system
```

---

## 📁 Project Structure

```
Graphics_Project/
├── main.cpp              # Entry point, game loop, input handling
├── src/
│   ├── planet.cpp/h      # Planet data & struct
│   ├── renderer.cpp/h    # All rendering (sun, planets, stars, etc.)
│   ├── camera.cpp/h      # Camera rotation & zoom
│   ├── lighting.cpp/h    # OpenGL lighting setup
│   ├── texture.cpp/h     # Texture loading (stb_image)
│   └── ui.cpp/h          # HUD, labels, info panel
├── textures/             # Planet texture images (.jpg)
├── Makefile
└── include/              # stb_image.h
```

---

## 📚 CG Topics Covered

| Topic | Usage |
|-------|-------|
| 3D Transformations | Planet orbit & self-rotation |
| Perspective Projection | `gluPerspective` |
| Camera / Viewing | `gluLookAt` with spherical coords |
| Phong Shading | Ambient + Diffuse + Specular per planet |
| Texture Mapping | Real planet textures via `stb_image` |
| Lighting Model | Sun as `GL_LIGHT0` point light |
| Animation | Timer-based 60 FPS loop |
| Z-Buffering | `GL_DEPTH_TEST` |
| Alpha Blending | Saturn ring + Sun glow |
| Geometric Primitives | Sphere, Line Loop, Triangle Strip, Points |
| Orthographic Projection | 2D HUD overlay |

---

## 🖼️ Textures

Textures sourced from [Solar System Scope](https://www.solarsystemscope.com/textures/) (free for educational use).

---

## 🏫 Academic Info

**Course:** Computer Graphics and Image Processing
**Department:** CSE, USTC
**Semester:** 7th Semester
