# Viva Preparation — 3D Solar System Simulation
## সম্ভাব্য প্রশ্ন ও উত্তর (বাংলায়)

---

## Part 1 — Code দেখে প্রশ্ন

---

### main.cpp

---

**Q: `glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH)` — এই line এর কাজ কী?**

তিনটা জিনিস set করছে:
- `GLUT_DOUBLE` — Double buffering চালু করছে। মানে দুটো buffer আছে — একটায় render হচ্ছে, অন্যটা screen এ দেখাচ্ছে। এতে screen flicker করে না।
- `GLUT_RGB` — Color mode RGB
- `GLUT_DEPTH` — Depth buffer চালু করছে, Z-buffering এর জন্য দরকার

---

**Q: `glutTimerFunc(16, update, 0)` — কেন 16?**

1000ms ÷ 16ms = প্রায় 60। মানে প্রতি 16 millisecond এ একবার update function call হচ্ছে, যেটা 60 FPS দেয়।

---

**Q: `gluPerspective(fov, aspect, 0.1, 500.0)` — এই চারটা parameter কী?**

- `fov` — Field of View, দেখার কোণ। সাধারণত 45°, telescope mode এ 10°
- `aspect` — width ÷ height। Window resize হলে যাতে distort না হয়
- `0.1` — Near clipping plane। এর চেয়ে কাছের কিছু দেখাবে না
- `500.0` — Far clipping plane। এর চেয়ে দূরের কিছু দেখাবে না

---

**Q: `glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)` — কেন দুটো clear করছ?**

প্রতি frame এ দুটো জিনিস clear করতে হয়:
- `GL_COLOR_BUFFER_BIT` — আগের frame এর color মুছে ফেলছে
- `GL_DEPTH_BUFFER_BIT` — আগের frame এর depth information মুছে ফেলছে। না মুছলে Z-buffering ভুল কাজ করবে

---

**Q: `glutSwapBuffers()` কী করে?**

Double buffering এ back buffer এ render হয়, front buffer screen এ দেখায়। `glutSwapBuffers()` দুটো swap করে — render করা frame screen এ আসে। এতে user কখনো incomplete frame দেখে না।

---

**Q: Telescope mode কীভাবে কাজ করে?**

T key চাপলে `targetFov` 45° থেকে 10° হয়। প্রতি frame এ:
```cpp
fov += (targetFov - fov) * 0.08f;
```
এই lerp দিয়ে smooth করে FOV কমে। FOV কম হলে দূরের জিনিস বড় দেখায় — telescope এর মতো।

---

### camera.cpp

---

**Q: `gluLookAt()` এর 9টা parameter কী?**

তিনটা group:
- প্রথম তিনটা — Camera এর position (কোথায় আছে)
- মাঝের তিনটা — Camera কোথায় তাকিয়ে আছে (target point)
- শেষের তিনটা — Up vector। `0, 1, 0` মানে Y axis উপরে

---

**Q: Camera rotation এ spherical coordinate কেন use করলে?**

Spherical coordinate (angleX, angleY, distance) দিয়ে camera কে একটা sphere এর surface এ রাখা যায় যেটা সবসময় center এর দিকে তাকিয়ে থাকে। Mouse drag করলে angle change হয়, scroll করলে distance change হয়।

```cpp
float ex = distance * cos(angleX) * sin(angleY);
float ey = distance * sin(angleX);
float ez = distance * cos(angleX) * cos(angleY);
```

---

**Q: Smooth camera এর জন্য lerp কী?**

Lerp মানে Linear Interpolation। Current value থেকে target value এর দিকে একটু একটু করে যাওয়া:
```cpp
camera.distance += (targetDistance - camera.distance) * 0.08f;
```
প্রতি frame এ difference এর 8% move করে। এতে camera smoothly zoom হয়, হঠাৎ jump করে না।

---

**Q: Double-click এ planet focus কীভাবে কাজ করে?**

দুটো click এর মধ্যে সময় check করি। 400ms এর মধ্যে একই planet এ দুইবার click হলে `focusPlanet()` call হয়। সেখানে camera এর `targetLookAt` সেই planet এর position এ set হয়, `targetDistance` planet এর size অনুযায়ী কমে। Lerp দিয়ে smoothly সেদিকে যায়।

---

### renderer.cpp

---

**Q: `glPushMatrix()` / `glPopMatrix()` কেন use করছ?**

OpenGL এ একটা matrix stack আছে। `glPushMatrix()` current matrix save করে, `glPopMatrix()` সেটা restore করে। একটা planet draw করার সময় translate/rotate করি, কিন্তু পরের planet এর জন্য সেই transformation বাদ দিতে হয় — তাই push/pop use করি।

---

**Q: Planet এর orbit কীভাবে calculate করছ?**

Parametric circle equation:
```cpp
float x = orbitRadius * cos(orbitAngle * PI / 180.0f);
float z = orbitRadius * sin(orbitAngle * PI / 180.0f);
```
প্রতি frame এ `orbitAngle += orbitSpeed` — এতে planet ধীরে ধীরে orbit করে।

---

**Q: Orbital inclination কীভাবে implement করলে?**

Y position এ inclination এর effect যোগ করলাম:
```cpp
float x = orbitRadius * cos(rad);
float y = orbitRadius * sin(rad) * sin(inclination);
float z = orbitRadius * sin(rad) * cos(inclination);
```
Inclination 0° হলে y=0, মানে flat plane। Pluto 17.1° তাই clearly উপরে-নিচে যায়।

---

**Q: Axial tilt কীভাবে implement করলে?**

Planet draw করার আগে Z-axis এ rotate করি:
```cpp
glRotatef(p.axialTilt, 0.0f, 0.0f, 1.0f);   // tilt
glRotatef(p.rotationAngle, 0.0f, 1.0f, 0.0f); // spin
```
Tilt আগে apply হয়, তারপর সেই tilted axis এ spin হয়। Uranus 97.8° তাই প্রায় শুয়ে spin করে।

---

**Q: `glDepthMask(GL_FALSE)` কেন use করলে Sun এর glow এ?**

Sun এর glow transparent sphere দিয়ে তৈরি। Depth write বন্ধ করলে এই sphere গুলো depth buffer update করে না — ফলে একটা আরেকটাকে block করে না এবং সব layer দেখা যায়। Render শেষে `glDepthMask(GL_TRUE)` দিয়ে আবার চালু করি।

---

**Q: Additive blending (`GL_SRC_ALPHA, GL_ONE`) কী?**

Normal blending এ নতুন color পুরনো color replace করে। Additive blending এ নতুন color পুরনো color এর সাথে যোগ হয়। Overlapping areas আরো bright হয় — এতে glow/fire effect হয়। Sun এর glow আর shooting star এ এটা use করেছি।

---

**Q: Earth এর night side city lights কীভাবে করলে?**

Earth এর উপর আরেকটা sphere draw করলাম night texture দিয়ে। Additive blend (`GL_ONE, GL_ONE`) use করলাম। Material এর diffuse 0 রাখলাম — মানে আলোর দিকে দেখাবে না। Emission দিলাম — মানে নিজে থেকে glow করবে। ফলে শুধু dark side এ city lights দেখা যায়।

---

**Q: Halley's Comet এর elliptical orbit কীভাবে করলে?**

Ellipse equation use করলাম:
```cpp
float cx = a * cos(rad) - (a - b);
float cz = b * sin(rad);
```
`a` হলো semi-major axis (লম্বা দিক), `b` হলো semi-minor axis (ছোট দিক)। `(a-b)` offset দিয়ে Sun কে একটা focus এ রাখলাম।

---

**Q: Kepler's Law কীভাবে implement করলে?**

Comet এর Sun থেকে distance calculate করি, তারপর speed সেই distance এর inverse অনুযায়ী set করি:
```cpp
float dist = sqrt(cx*cx + cz*cz);
halley.speed = 0.35f / (dist * 0.08f + 0.5f);
```
Sun এর কাছে dist কম → speed বেশি। দূরে dist বেশি → speed কম। এটাই Kepler's Second Law।

---

### lighting.cpp

---

**Q: `GL_LIGHT0` কোথায় রেখেছ এবং কেন?**

Origin (0,0,0) এ রেখেছি কারণ Sun সেখানে আছে। Sun হলো solar system এর আলোর উৎস — তাই Sun এর position এ point light রাখলে realistic lighting হয়।
```cpp
GLfloat position[] = {0.0f, 0.0f, 0.0f, 1.0f};
```
শেষের `1.0f` মানে point light। `0.0f` হলে directional light হতো।

---

**Q: Ambient, Diffuse, Specular এর পার্থক্য কী?**

- **Ambient** — সব দিক থেকে সমান আলো। Shadow area তেও কিছুটা দেখা যায়। আমরা 0.18 রেখেছি — space অন্ধকার তাই কম।
- **Diffuse** — Surface এর angle অনুযায়ী আলো। Sun এর দিকে বেশি, বিপরীতে কম। এটাই main lighting।
- **Specular** — Shiny highlight। Planet এর surface এ চকচকে spot।

---

### texture.cpp

---

**Q: `stb_image.h` কী?**

Sean Barrett এর লেখা single header image loading library। JPG, PNG সহ অনেক format support করে। আমরা এটা দিয়ে planet এর texture file load করে OpenGL এ দিয়েছি। External dependency নেই — শুধু একটা `.h` file।

---

**Q: `stbi_set_flip_vertically_on_load(true)` কেন?**

OpenGL এর texture coordinate এর Y-axis নিচ থেকে শুরু হয়, কিন্তু image file এ Y-axis উপর থেকে শুরু হয়। Flip না করলে texture উল্টো দেখায়।

---

**Q: `glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)` কী করে?**

Texture filtering set করছে। `GL_LINEAR` মানে bilinear filtering — texture zoom in/out করলে smooth দেখাবে, pixelated না।

---

### audio.cpp

---

**Q: Sound কোথা থেকে আসছে? কোনো file নেই কেন?**

Sound procedurally generate করেছি — math দিয়ে sound তৈরি করেছি। Sine wave equation use করে:
```cpp
wave += 0.30 * sin(2.0 * PI * 55.0 * t);   // 55Hz bass
wave += 0.20 * sin(2.0 * PI * 110.0 * t);  // octave
```
এই waves গুলো mix করে space ambient sound তৈরি হয়। কোনো external audio file দরকার নেই।

---

**Q: LFO কী এবং কেন use করলে?**

LFO মানে Low Frequency Oscillator। খুব ধীরে oscillate করে (0.05Hz) — এটা main sound এর volume modulate করে:
```cpp
double lfo = 0.75 + 0.25 * sin(2.0 * PI * 0.05 * t);
wave *= lfo;
```
এতে sound এ breathing/pulsing effect আসে, static না হয়ে alive মনে হয়।

---

## Part 2 — General Concept Questions

---

**Q: তোমার project এ কোন কোন CG topic use করেছ?**

মোট ১৩টা topic:
1. **3D Transformations** — Planet orbit, self-rotation, axial tilt, orbital inclination
2. **Perspective Projection** — `gluPerspective` দিয়ে 3D → 2D
3. **Orthographic Projection** — HUD overlay এ `gluOrtho2D`
4. **Camera/Viewing** — `gluLookAt` দিয়ে spherical coordinate camera
5. **Phong Shading** — Ambient + Diffuse + Specular প্রতিটা planet এ
6. **Texture Mapping** — Real planet texture `stb_image` দিয়ে load
7. **Lighting Model** — Sun কে `GL_LIGHT0` point light হিসেবে
8. **Alpha Blending** — Ring, atmosphere, glow, cloud, night lights
9. **Animation** — 60 FPS timer loop দিয়ে
10. **Z-Buffering** — `GL_DEPTH_TEST` দিয়ে correct depth rendering
11. **Geometric Primitives** — Sphere, Line Loop, Triangle Strip, Points
12. **Kepler's Law** — Comet এর speed Sun এর distance অনুযায়ী
13. **Particle System** — Asteroid belt, shooting stars

---

**Q: Double buffering কী এবং কেন দরকার?**

দুটো frame buffer থাকে — front আর back। Rendering হয় back buffer এ, display হয় front buffer এ। Rendering শেষ হলে swap হয়। এতে user কখনো incomplete frame দেখে না — flickering বন্ধ হয়।

---

**Q: Z-buffering কী?**

প্রতিটা pixel এর জন্য depth value store করা হয়। নতুন object draw করার সময় check করে — এই pixel এ আগে কোনো closer object আছে কিনা। থাকলে draw করে না। এতে correct depth ordering হয় — কাছের object দূরের object কে cover করে।

---

**Q: Phong shading model কী?**

তিনটা component এর sum:
- **Ambient** = Ka × Ia — constant background light
- **Diffuse** = Kd × Id × (N·L) — surface normal আর light direction এর dot product
- **Specular** = Ks × Is × (R·V)^n — reflection vector আর view vector

N = normal, L = light direction, R = reflection, V = view direction, n = shininess

---

**Q: Texture mapping কীভাবে কাজ করে?**

2D image কে 3D surface এ map করা। `gluSphere` automatically texture coordinate (u,v) generate করে — u হলো longitude (0 থেকে 1), v হলো latitude (0 থেকে 1)। OpenGL সেই coordinate অনুযায়ী image এর pixel নিয়ে sphere এর surface এ লাগায়।

---

**Q: Alpha blending কী?**

দুটো color mix করার process। Formula:
```
output = src_color * src_alpha + dst_color * (1 - src_alpha)
```
Alpha 0 = সম্পূর্ণ transparent, Alpha 1 = সম্পূর্ণ opaque। Ring, cloud, atmosphere এ এটা use করেছি।

---

**Q: Particle system কী?**

অনেকগুলো ছোট object একসাথে simulate করা। Asteroid belt এ 300টা random position এ ছোট sphere রেখেছি। Shooting star এ 5টা particle spawn হয়, life বাড়ার সাথে fade out হয়।

---

**Q: তোমার project এ কে কী করেছে?**

- **Member 1** — Rendering, textures, Sun glow, Saturn/Uranus ring, asteroid belt, shooting stars, Milky Way background, Earth clouds, night lights, atmosphere glow, Halley's Comet
- **Member 2** — Planet orbital mechanics, camera system, keyboard/mouse input, UI panel, speed control, pause/resume, arrow key navigation, telescope mode, screenshot

---

*এগুলো মুখস্থ না করে বুঝে রাখো — professor follow-up question করলে বুঝে থাকলে answer করতে পারবে।*
