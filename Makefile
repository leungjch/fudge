CXX= g++
CXXFLAGS=-std=c++17 -g -MMD 
OBJECTS=main.o graph.o node.o vec3d.o universe.o rgb.o

DEPENDS=${OBJECTS:.o=.d}
EXEC=fdg


${EXEC}: ${OBJECTS}
	${CXX} ${OBJECTS} -o ${EXEC}
	rm ${OBJECTS} ${DEPENDS}
-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${DEPENDS} ${EXEC}