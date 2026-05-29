# 🌍 3D Solar System Simulation

A real-time interactive 3D Solar System simulation built from scratch with **C++ and OpenGL**.

![OpenGL](https://img.shields.io/badge/OpenGL-GLUT-blue) ![C++](https://img.shields.io/badge/C++-17-brightgreen) ![Platform](https://img.shields.io/badge/Platform-Linux-orange) ![Audio](https://img.shields.io/badge/Audio-OpenAL-purple)

---

## 📸 Features

### Celestial Bodies
- ☀️ Sun with animated fiery glow and corona effect
- 🪐 All 9 planets (Mercury → Pluto) with **real surface textures**
- 🌍 Earth with **cloud layer** + **night city lights**
- 🌙 Earth's Moon, Mars's Phobos & Deimos, Jupiter's Io, Europa & Ganymede
- 💍 Saturn's wide ring + Uranus's thin bluish ring
- ☄️ Asteroid belt between Mars and Jupiter
- ☄️ **Halley's Comet** with elliptical orbit and animated tail (Kepler's Law)
- ⭐ Milky Way background texture + 1500 point stars
- 🌠 Random shooting stars

### Scientific Accuracy
- Real axial tilt values (Earth 23.5°, Uranus 97.8°, etc.)
- Real orbital inclination (Mercury 7°, Pluto 17.1°, etc.)
- Kepler's Law — comet speeds up near Sun, slows down far away
- Relative planet sizes and orbital distances

### Visual Effects
- 🌈 Atmosphere glow — Earth (blue), Venus (yellow), Mars (red)
- 💡 Multi-layer additive Sun bloom
- 🌙 Earth night side city lights
- ☁️ Semi-transparent rotating cloud layer on Earth

### Interaction
- 🎮 Full camera control (rotate, zoom, focus)
- 🔭 Telescope mode (FOV zoom)
- ⏸️ Pause/Resume, speed control (0.1x → 20x)
- 🖱️ Double-click planet to focus camera
- ⬅️➡️ Arrow keys to navigate between planets
- 📷 Screenshot (F1)
- 🔭 Toggle orbit lines and planet labels

### Audio
- 🔊 Procedurally generated space ambient sound (OpenAL)
- Deep drone with LFO modulation — no external audio file needed

---

## 🎮 Controls

| Key / Action | Function |
|-------------|---------|
| Mouse Left Drag | Rotate camera |
| Scroll Up / `W` | Zoom In |
| Scroll Down / `S` | Zoom Out |
| `SPACE` | Pause / Resume |
| `=` / `-` | Speed Up / Slow Down |
| `T` | Toggle Telescope Mode |
| `O` | Toggle orbit lines |
| `L` | Toggle planet labels |
| `R` | Reset camera |
| `← →` Arrow Keys | Navigate between planets |
| Double-Click Planet | Focus camera on planet |
| `F1` | Save Screenshot |
| `ESC` | Exit |

---

## 🛠️ Build & Run

### Install Dependencies
```bash
sudo apt-get install freeglut3-dev libgl1-mesa-dev libglu1-mesa-dev libopenal-dev
```

### Build
```bash
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

## 📁 Project Structure

```
Graphics_Project/
├── main.cpp                  # Entry point, game loop, input handling
├── src/
│   ├── planet.cpp/h          # Planet data & struct (18 properties each)
│   ├── renderer.cpp/h        # All rendering (sun, planets, comet, etc.)
│   ├── camera.cpp/h          # Smooth camera with focus & lerp
│   ├── lighting.cpp/h        # OpenGL lighting setup
│   ├── texture.cpp/h         # Texture loading (stb_image)
│   ├── audio.cpp/h           # Procedural space ambient (OpenAL)
│   └── ui.cpp/h              # HUD, labels, info panel
├── textures/                 # Planet texture images (.jpg)
├── include/                  # stb_image.h
├── Makefile
├── PROJECT_DOCUMENTATION.md
└── PROJECT_COMPLETION_REPORT.md
```

---

## 📚 CG Topics Covered

| Topic | Usage |
|-------|-------|
| 3D Transformations | Orbit, self-rotation, axial tilt, orbital inclination |
| Perspective Projection | `gluPerspective` with dynamic FOV |
| Orthographic Projection | 2D HUD overlay |
| Camera / Viewing | `gluLookAt` with spherical coords + smooth lerp |
| Phong Shading | Ambient + Diffuse + Specular per planet |
| Texture Mapping | Real planet textures via `stb_image` |
| Lighting Model | Sun as `GL_LIGHT0` point light |
| Alpha Blending | Rings, atmosphere, glow, clouds, night lights |
| Animation | Timer-based 60 FPS loop |
| Z-Buffering | `GL_DEPTH_TEST` + `glDepthMask` |
| Geometric Primitives | Sphere, Line Loop, Triangle Strip, Points |
| Kepler's Law | Comet speed varies with Sun distance |
| Particle System | Asteroid belt, shooting stars |

---

## 🖼️ Textures

Textures sourced from [Solar System Scope](https://www.solarsystemscope.com/textures/) (free for educational use).

---

## 🏫 Academic Info

**Course:** Computer Graphics and Image Processing
**Department:** CSE, USTC
**Semester:** 7th Semester
