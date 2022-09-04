CXX= g++
CXXFLAGS=-std=c++17 -fopenmp -MMD 
OBJECTS=main.o graph.o node.o vec3d.o universe.o rgb.o mycamera.o

DEPENDS=${OBJECTS:.o=.d}
EXEC=fdg


${EXEC}: ${OBJECTS}
	${CXX} ${OBJECTS} -o ${EXEC} -I/usr/X11R6/include -L/usr/X11R6/lib \
    -lGL -lGLEW -lSDL2 -lGLU -lglfw  -fopenmp
	rm ${OBJECTS} ${DEPENDS}
-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${DEPENDS} ${EXEC}