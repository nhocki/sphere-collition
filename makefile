CC = g++
CFLAGS = -Wall
PROG = ejemplo
LIBS = -lglut

SRCS = main.cpp Camera.cpp Loader.cpp math/Vector3.cpp math/Utility.cpp objects/Sphere.cpp objects/Wall.cpp

all: $(SRCS)
	 $(CC) $(CFLAGS) $(LIBS) $(SRCS) -o$(PROG)
