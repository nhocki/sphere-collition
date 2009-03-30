ejemplo.o: main.cpp
	g++ -lglut main.cpp Camera.cpp math/Utility.cpp math/Vector3.cpp objects/Sphere.h objects/Wall.h -oejemplo

