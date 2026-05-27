CXX     = g++
CXXFLAGS= -std=c++17 -O2 -Wall -Wno-deprecated-declarations -I.
LIBS    = -lGL -lGLU -lglut -lm -lopenal
SRC     = main.cpp src/planet.cpp src/renderer.cpp src/camera.cpp src/lighting.cpp src/texture.cpp src/ui.cpp src/audio.cpp
TARGET  = solar_system

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)
