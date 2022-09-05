#if __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glu.h>
#include <iostream>
#include "graph.h"
#include "universe.h"
#include "vec3d.h"
#include "mycamera.h"
#include <GLFW/glfw3.h>
#include <ctime>
#include <assert.h>
#ifndef __EMSCRIPTEN__
    #include <omp.h>
#endif

using namespace std;
bool movingUp = false;  // Whether or not we are moving up or down
float yLocation = 0.0f; // Keep track of our position on the y axis.
int n_iterations = 200;

Graph graph;
MyCamera camera(Vec3D(0, 0, 35));

double mouseX = 0;
double mouseY = 0;
double mouseScroll = 0;
bool mouseDown = false;
bool mouseDownFirst = false;
bool autoRotateX = false;

Universe universe(graph,
                  0.05, // dt
                  0.5,  // repulsion
                  1.0,  // spring
                  1.0   // damping
);


void init_graph()
{
    int n1 = graph.add_node("A");
    int n2 = graph.add_node("B");
    std::srand(time(NULL));
    for (int i = 0; i < 10; i++)
    {
        int n3 = graph.add_node("C");
        int rd = rand() % (graph.node_list.size());
        // if (rd % 2 == 0)
        {
            graph.add_edge(n3, rd);
        }
    }

    // graph.node_list[n1].pos = Vec3D(-1.0, 0, 0);
    // graph.node_list[n2].pos = Vec3D(1.0, 0, 0);
    graph.add_edge(n1, n2);

    universe.set_graph(graph);
}

void draw_graph(float yloc)
{
    static int n_nodes = universe.graph.adj_list.size();

    glColor3f(1.0, 1.0, 1.0);
    // Populate the edge positions
    for (int i = 0; i < universe.graph.adj_list.size(); i++)
    {
        Node node_i = universe.graph.node_list[i];
        unordered_set<int> neighbors = universe.graph.adj_list[i];
        for (auto j = neighbors.begin(); j != neighbors.end(); ++j)
        {
            // Draw a line from node_i to node_j
            Node node_j = universe.graph.node_list[*j];
            glBegin(GL_LINES);
            glVertex3f(node_i.pos.x, node_i.pos.y, node_i.pos.z);
            glVertex3f(node_j.pos.x, node_j.pos.y, node_j.pos.z);
            glEnd();
        }
    }
    for (int i = 0; i < n_nodes; i++)
    { 
        Node nd = universe.graph.node_list[i];

        // // Draw as spheres
        // glColor3f(nd.color.r / 255.0,
        //           nd.color.g / 255.0,
        //           nd.color.b / 255.0);
        // GLUquadric *quad;
        // quad = gluNewQuadric();
        // // Radius is determined by a node's degree
        // // Apply nonlinear scaling
        // double radius = log(nd.degree+1)*0.5;
        // glTranslatef(nd.pos.x, nd.pos.y, nd.pos.z);
        // gluSphere(quad, radius, 25, 10);
        // glTranslatef(-nd.pos.x, -nd.pos.y, -nd.pos.z);

        // // Draw as points
        glPointSize(10);
        glColor3f(nd.color.r/255.0,
                nd.color.g/255.0,
                nd.color.b/255.0);
        glBegin(GL_POINTS);
        glVertex3f(nd.pos.x, nd.pos.y, nd.pos.z);
        glEnd();
    }
}
void render(void)
{

    glClearColor(0.0f, 1.0f, 0.0f, 1.0f); // Clear the background of our window to red
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);         // Clear the colour buffer (more buffers later on)
    glLoadIdentity();                     // Load the Identity Matrix to reset our drawing locations

    // Set the camera
    gluLookAt(camera.pos.x, 0.0f, camera.pos.z, camera.pos.x, 0.0f, camera.pos.z - 1.0f, 0.0f, 1.0f, 0.0f);
    glRotatef(camera.angle.x, 1, 0, 0);
    glRotatef(camera.angle.y, 0, 1, 0);
    glRotatef(camera.angle.z, 0, 0, 1);

    draw_graph(yLocation);
    glTranslatef(0.0f, 0.0f, yLocation); // Translate our object along the y axis
}

typedef int32_t i32;
typedef uint32_t u32;
typedef int32_t b32;

#define WIN_WIDTH 1000
#define WIN_HEIGHT 1000


static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}
 
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    if (key == GLFW_KEY_W) {
        camera.angle.x += 1.0;
    }
    if (key == GLFW_KEY_S) {
        camera.angle.x -=  1.0;
    }
    if (key == GLFW_KEY_A) {
        camera.angle.y += 1.0;
    }
    if (key == GLFW_KEY_S) {
        camera.angle.y -= 1.0;
    }
    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        autoRotateX = !autoRotateX;
    }
}
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    // If first time mouse down, reset the mouse positions
    // This prevents the camera from jerking when initially rotating
    if (mouseDownFirst) {
        mouseX = xpos;
        mouseY = ypos;
        mouseDownFirst = false;
    }
    // Rotate the camera around the origin
    // Only rotate if user is holding down mouse click
    if (mouseDown) {
        double dx = xpos - mouseX;
        double dy = ypos - mouseY;
        mouseX = xpos;
        mouseY = ypos;
        camera.angle.x -= dy;
        camera.angle.y -= dx;
    }
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.pos.z -= yoffset;
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        mouseDown = true;
        mouseDownFirst = true;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        mouseDown = false;
}


int main(int ArgCount, char **Args)
{
    srand(time(NULL));
    #ifndef __EMSCRIPTEN__
    // Test if openMP is working
    #pragma omp parallel for
    for (int i = 0; i < 16; ++i)
    {
        cout << omp_get_max_threads() << endl;
        printf("Thread %d works with idx %d\n", omp_get_thread_num(), i);
    }
    #endif

    init_graph();

    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);

    window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    glfwMakeContextCurrent(window);
    // gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);

    /* Set rendering settings */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (GLfloat)WIN_WIDTH / (GLfloat)WIN_HEIGHT, 1.0, 400.0); // Set the Field of view angle (in degrees), the aspect ratio of our window, and the new and far planes
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable(GL_DEPTH_TEST);

    b32 running = 1;
    b32 fullScreen = 0;

    double timeDelta = 1000 / 60;
    double timeAccumulator = 0;
    double timeSimulatedThisIteration = 0;
    u32 frames = 0;
    u32 currentTime = glfwGetTime();
    u32 startTime = glfwGetTime();
    u32 initTime = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {

        timeSimulatedThisIteration = 0;
        startTime = glfwGetTime();
        ++frames;

        // Run update logic
        if (frames < n_iterations)
        {
            universe.update(1.0 / timeDelta);
        }

        currentTime = glfwGetTime();
        cout << "Update time: " << currentTime - startTime << endl;

        if (autoRotateX) {
            camera.angle.y += 1.0;
        }

        glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
        // glClearColor(0.f, 0.f, 0.f, 0.f);
        // glClear(GL_COLOR_BUFFER_BIT);
        u32 renderStart = (float) glfwGetTime();
        render();
        u32 renderEnd = glfwGetTime();
        cout << "Render time: " << renderEnd - renderStart << endl;
        // Render();
        currentTime = glfwGetTime();
        timeAccumulator += currentTime - startTime;
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    currentTime = glfwGetTime();
    if (currentTime > initTime)
    {
        printf("%2.2f frames per second\n",
               ((double)frames * 1000) / (currentTime - initTime));
    }

    glfwDestroyWindow(window);
    glfwTerminate();


    return 0;
}