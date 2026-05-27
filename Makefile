CXX     = g++
CXXFLAGS= -std=c++17 -O2 -Wall -Wno-deprecated-declarations -I.
LIBS    = -lGL -lGLU -lglut -lm
SRC     = main.cpp src/planet.cpp src/renderer.cpp src/camera.cpp src/lighting.cpp src/texture.cpp src/ui.cpp
TARGET  = solar_system

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)
