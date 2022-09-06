CXX= g++
CXXFLAGS=-std=c++17 -fopenmp -MMD 
OBJECTS=main.o graph.o node.o utils/vec3d.o utils/rgb.o \
	 universe.o  mycamera.o \
	 draw_utils/solid_sphere.o draw_utils/line.o \
	 imgui/imgui.o imgui/imgui_impl_opengl3.o imgui/imgui_draw.o imgui/imgui_impl_glfw.o imgui/imgui_tables.o imgui/imgui_widgets.o


DEPENDS=${OBJECTS:.o=.d}
EXEC=fdg


${EXEC}: ${OBJECTS}
	${CXX} ${OBJECTS} -o ${EXEC} -I/usr/X11R6/include -L/usr/X11R6/lib \
    -lGL -lGLEW -lGLU -fopenmp -lglfw

	rm ${OBJECTS} ${DEPENDS}

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${DEPENDS} ${EXEC}