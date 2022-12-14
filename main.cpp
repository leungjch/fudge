#if __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glu.h>
#include <iostream>
#include "graph.h"
#include "universe.h"
#include "utils/vec3d.h"
#include "draw_utils/line.h"
#include "draw_utils/solid_sphere.h"
#include "mycamera.h"
#include <GLFW/glfw3.h>
#include <ctime>
#include <assert.h>
#include <functional>
#include "glm/glm/ext.hpp"
#ifndef __EMSCRIPTEN__
#include <omp.h>
#endif

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"

typedef int32_t i32;
typedef uint32_t u32;
typedef int32_t b32;

#define WIN_WIDTH 1024
#define WIN_HEIGHT 1024

using namespace std;
bool movingUp = false;  // Whether or not we are moving up or down
float yLocation = 0.0f; // Keep track of our position on the y axis.
int n_iterations = INT_FAST32_MAX;

Graph graph;
MyCamera camera(WIN_WIDTH, WIN_HEIGHT);

double mouseX = 0;
double mouseY = 0;
double mouseScroll = 0;
float scroll_sensitivity = 1.0f;
float drag_sensitivity = 1.5;
float zoom = 0.0f;
bool mouseDown = false;
bool mouseDownFirst = false;
bool autoRotateX = true;
bool show_degree = true;
float MIN_DISTANCE_ORIGIN = 5.0f;
float zoom_step = 2.5f;
glm::mat4 projection;
glm::mat4 view;

int n_random_nodes = 100;

// Universe settings
float timeDelta = 0.2;
float repulsion_force = 1.0f;
float spring_force = 1.0f;
float damping_coefficient = 0.5f;
float gravitational_force = 0.5f;

Universe universe(graph,
                  timeDelta,
                  repulsion_force,
                  spring_force,
                  damping_coefficient,
                  gravitational_force
);

SolidSphere sphere(
    10.0f, 12, 24);
Line line(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
void init_graph()
{
    graph.adj_list.clear();
    graph.node_list.clear();

    std::srand(time(NULL));
    for (int i = 0; i < n_random_nodes; i++)
    {
        int n3 = graph.add_node("C");
        int rd = rand() % (graph.node_list.size());
        graph.add_edge(n3, rd);
    }

    universe.set_graph(graph);
}

void draw_graph(float yloc)
{
    static int n_nodes = universe.graph.adj_list.size();

    // glColor3f(1.0, 1.0, 1.0);
    // Populate the edge positions
    for (int i = 0; i < universe.graph.adj_list.size(); i++)
    {

        Node node_i = universe.graph.node_list[i];
        unordered_set<int> neighbors = universe.graph.adj_list[i];
        for (auto j = neighbors.begin(); j != neighbors.end(); ++j)
        {
            // Draw a line from node_i to node_j
            Node node_j = universe.graph.node_list[*j];
            line.setVertices(
                glm::vec3(node_i.pos.x, node_i.pos.y, node_i.pos.z),
                glm::vec3(node_j.pos.x, node_j.pos.y, node_j.pos.z));
            line.setMVP(projection * view);
            line.draw();
        }
    }
    for (int i = 0; i < n_nodes; i++)
    {
        Node nd = universe.graph.node_list[i];

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(nd.pos.x, nd.pos.y, nd.pos.z));
        float radius = 1.0f;
        // Scale radius based on degree
        if (show_degree)
        {
            radius = min(nd.degree / 2.5, 1.5);
        }
        glm::vec3 scale = glm::vec3(radius, radius, radius);
        model = glm::scale(model, scale);
        sphere.setColor(nd.color.color);
        sphere.setMVP(model, view, projection);
        sphere.draw();
    }
}
void render(void)
{

    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);               // Clear the background of our window to red
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the colour buffer (more buffers later on)

    view = camera.get_view_mat();

    draw_graph(yLocation);
}

static void error_callback(int error, const char *description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    if (key == GLFW_KEY_W)
    {
        camera.update_view_mat(0.1, 0);
    }
    if (key == GLFW_KEY_S)
    {
        camera.update_view_mat(-0.1, 0);
    }
    if (key == GLFW_KEY_A)
    {
        camera.update_view_mat(0, 0.1);
    }
    if (key == GLFW_KEY_D)
    {
        camera.update_view_mat(0, -0.1);
    }
    if (key == GLFW_KEY_R && action == GLFW_PRESS)
    {
        autoRotateX = !autoRotateX;
    }
}
static void cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
{
    // If first time mouse down, reset the mouse positions
    // This prevents the camera from jerking when initially rotating
    if (mouseDownFirst)
    {
        cout << "Mouse clicked" << endl;
        mouseX = xpos;
        mouseY = ypos;
        mouseDownFirst = false;
    }
    // Rotate the camera around the origin
    // Only rotate if user is holding down mouse click
    if (mouseDown)
    {
        double dx = (xpos - mouseX) * drag_sensitivity;
        double dy = (ypos - mouseY) * drag_sensitivity;
        mouseX = xpos;
        mouseY = ypos;
        camera.update_view_mat(dx, dy);
    }
}
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    // yoffset is -1 or 1
    cout << "Scroll clicked" << endl;
    // If zooming in, make sure not to zoom past the origin
    if (glm::distance(camera.pos, glm::vec3(0.0f)) > MIN_DISTANCE_ORIGIN)
    {
        camera.pos += (float)yoffset * scroll_sensitivity * camera.view_dir();
    // Push the camera back if it is zoomed in too far
    } else {
        camera.pos -= camera.view_dir() * scroll_sensitivity;
    }
}
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    ImGuiIO &io = ImGui::GetIO();
    io.AddMouseButtonEvent(button, action);

    // Disable if user is hovering over imgui
    if (!io.WantCaptureMouse)

    {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
            mouseDown = true;
        mouseDownFirst = true;
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
            mouseDown = false;
    }
}
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    camera.set_viewport(width, height);
}

std::function<void()> loop;
void main_loop() { loop(); }

int main(int ArgCount, char **Args)
{

    cout << "Starting fudge..." << endl;
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

    cout << "Initiating graph..." << endl;
    init_graph();
    cout << "Done initiating graph with n=" << universe.graph.node_list.size() << endl;

    GLFWwindow *window;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);

    window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit())
    {
        cout << "GLEW INIT FAILED, EXITING" << endl;
        exit(EXIT_FAILURE);
    }

    // gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);

    /* Set rendering settings */
    projection = glm::perspective(
        // FOV & aspect
        glm::radians<float>(60.0f), (GLfloat)WIN_WIDTH / (GLfloat)WIN_HEIGHT,
        // Near and far planes
        0.1f, 500.0f);

    sphere.init();
    line.init();

    glEnable(GL_DEPTH_TEST);

    b32 running = 1;
    b32 fullScreen = 0;

    double timeAccumulator = 0;
    double timeSimulatedThisIteration = 0;
    double currentTime = glfwGetTime();
    double startTime = glfwGetTime();
    double initTime = glfwGetTime();

    // Setup imgui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 300 es");

    // Render loop
    loop = [&]
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        timeSimulatedThisIteration = 0;
        startTime = glfwGetTime();

        // Run update logic
        if (universe.n_iterations < n_iterations)
        {
            universe.update(timeDelta);
        }

        currentTime = glfwGetTime();
        // cout << "Update time: " << currentTime - startTime << endl;

        if (autoRotateX)
        {
            camera.update_view_mat(2.5, 0);
        }

        // double renderStart = (float)glfwGetTime();
        render();
        // double renderEnd = glfwGetTime();
        // cout << "Render time: " << renderEnd - renderStart << endl;

        ImGui::Begin("Settings", NULL, ImGuiWindowFlags_AlwaysAutoResize);
        if (ImGui::SliderFloat("Spring force", &spring_force, 0.0f, 1.0f))
        {
            universe.spring_k = spring_force;
        }
        if (ImGui::SliderFloat("Repulsion force", &repulsion_force, 0.0f, 1.0f))
        {
            universe.repulsion = repulsion_force;
        }
        if (ImGui::SliderFloat("Gravitational force", &gravitational_force, 0.0f, 1.0f))
        {
            universe.gravity = gravitational_force;
        }
        if (ImGui::SliderFloat("Damping coefficient", &damping_coefficient, 0.0f, 1.0f))
        {
            universe.damping = damping_coefficient;
        }
        if (ImGui::SliderFloat("Render speed", &timeDelta, 0.001f, 0.40f))
        {
            universe.dt = timeDelta;   
        }

        // ImGui::SliderInt("Render time", &n_iterations, 50, 500);
        if (ImGui::Button("+"))
        {
            if (glm::distance(camera.pos, glm::vec3(0.0f)) > MIN_DISTANCE_ORIGIN)
            {
                camera.pos += camera.view_dir()*zoom_step;
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("-"))
        {
            camera.pos -= camera.view_dir()*zoom_step;
        }
        ImGui::SameLine();
        ImGui::Text("Zoom");

        
        ImGui::Checkbox("Show vertex degree", &show_degree);
        ImGui::Checkbox("Orbit camera", &autoRotateX);
        if (ImGui::Button("Random graph"))
        {
            init_graph();
        }
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        currentTime = glfwGetTime();
        timeAccumulator += currentTime - startTime;
        glfwSwapBuffers(window);
        glfwPollEvents();
    };

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(main_loop, 0, true);
#else
    while (!glfwWindowShouldClose(window))
        main_loop();
#endif

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}