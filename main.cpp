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
#include <functional>
#include "glm/glm/ext.hpp"
#ifndef __EMSCRIPTEN__
    #include <omp.h>
#endif

using namespace std;
bool movingUp = false;  // Whether or not we are moving up or down
float yLocation = 0.0f; // Keep track of our position on the y axis.
int n_iterations = 50;

Graph graph;
MyCamera camera(10.0);

double mouseX = 0;
double mouseY = 0;
double mouseScroll = 0;
bool mouseDown = false;
bool mouseDownFirst = false;
bool autoRotateX = false;

glm::mat4 projection;
glm::mat4 view;

Universe universe(graph,
                  0.05, // dt
                  1.0,  // repulsion
                  1.0,  // spring
                  1.0   // damping
);

// GL_LINES is deprecated, use this instead 
// https://stackoverflow.com/questions/14486291/how-to-draw-line-in-opengl
class Line {
    int shaderProgram;
    unsigned int VBO, VAO;
    vector<float> vertices;
    glm::vec3 startPoint;
    glm::vec3 endPoint;
    glm::mat4 MVP = glm::mat4(1.0);
    glm::vec3 lineColor;
public:
    Line(glm::vec3 start, glm::vec3 end) {
 
        startPoint = start;
        endPoint = end;
        lineColor = glm::vec3(1,1,1);
 
        const char *vertexShaderSource = "#version 300 es \n"
            "layout (location = 0) in vec3 aPos;\n"
            "uniform mat4 MVP;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = MVP * vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
            "}\0";
        const char *fragmentShaderSource = "#version 300 es \n"
            "precision mediump float;\n"
            "out vec4 FragColor;\n"
            "uniform vec3 color;\n"
            "void main()\n"
            "{\n"
            "   FragColor = vec4(color, 1.0f);\n"
            "}\n\0";
 
        int success;
        char infoLog[512];
        // vertex shader
        int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);
        // check for shader compile errors
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

        // check for shader compile errors
        if(!success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        };
        // fragment shader
        int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

        // check for shader compile errors
        if(!success)
        {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        };

        // link shaders
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        // check for linking errors
 

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
 
        vertices = {
             start.x, start.y, start.z,
             end.x, end.y, end.z,
 
        };
        
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
 
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)*vertices.size(), vertices.data(), GL_STATIC_DRAW);
 
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
 
        glBindBuffer(GL_ARRAY_BUFFER, 0); 
        glBindVertexArray(0); 
 
    }
 
    int setMVP(glm::mat4 mvp) {
        MVP = mvp;
        return 1;
    }
 
    int setColor(glm::vec3 color) {
        lineColor = color;
        return 1;
    }
 
    int draw() {
        glUseProgram(shaderProgram);
 
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "MVP"), 1, GL_FALSE, &MVP[0][0]);
        glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1, &lineColor[0]);
 
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINES, 0, 2);
        return 1;
    }
 
    ~Line() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteProgram(shaderProgram);
    }
};

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
            // glBegin(GL_LINES);
            // glVertex3f(node_i.pos.x, node_i.pos.y, node_i.pos.z);
            // glVertex3f(node_j.pos.x, node_j.pos.y, node_j.pos.z);
            // glEnd();
            Line line(
                glm::vec3(node_i.pos.x, node_i.pos.y, node_i.pos.z),
                glm::vec3(node_j.pos.x, node_j.pos.y, node_j.pos.z)
            );
            line.setMVP(projection * view);
            line.draw();

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
        // glPointSize(10);
        // glColor3f(nd.color.r/255.0,
        //         nd.color.g/255.0,
        //         nd.color.b/255.0);
        // glBegin(GL_POINTS);
        // glVertex3f(nd.pos.x, nd.pos.y, nd.pos.z);
        // glEnd();
    }
}
void render(void)
{

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Clear the background of our window to red
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);         // Clear the colour buffer (more buffers later on)

    // update camera position (rotating)
    // camera.position = glm::vec3(10*cos(glm::radians(camera.angle.x)), 10, 10*sin(glm::radians(camera.angle.z)));
    float camX = sin(camera.angle.x) * camera.distance;
    float camZ = cos(camera.angle.y) * camera.distance;


    camera.position = glm::vec3(camX, 0.0, camZ);
    view = glm::lookAt(camera.position, 
    glm::vec3(0,0,0),
    glm::vec3(0,1,0));


    draw_graph(yLocation);
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
        camera.angle.x += 0.1;
    }
    if (key == GLFW_KEY_S) {
        camera.angle.x -=  0.1;
    }
    if (key == GLFW_KEY_A) {
        camera.angle.y += 0.1;
    }
    if (key == GLFW_KEY_D) {
        camera.angle.y -= 0.1;
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
    cout << "Mouse clicked" << endl;
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
        camera.angle.x -= dy/10;
        camera.angle.y -= dx/10;
    }
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    cout << "Scroll clicked" << endl;
    camera.distance = max(0.1, camera.distance-yoffset);
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        mouseDown = true;
        mouseDownFirst = true;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        mouseDown = false;
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
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
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE; //Ensure it get all pointers
    if ( GLEW_OK != glewInit() )
    {
        //glewInit failed, something is seriously wrong.
        return false; //or any handling here
    }


    // gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);

    /* Set rendering settings */
    projection = glm::perspective(
    // FOV & aspect
    glm::radians<float>(60.0f), (GLfloat)WIN_WIDTH / (GLfloat)WIN_HEIGHT,
    // Near and far planes
    0.1f, 100.0f);

    glEnable(GL_DEPTH_TEST);

    b32 running = 1;
    b32 fullScreen = 0;

    double timeDelta = 1000 / 60;
    double timeAccumulator = 0;
    double timeSimulatedThisIteration = 0;
    u32 frames = 0;
    double currentTime = glfwGetTime();
    double startTime = glfwGetTime();
    double initTime = glfwGetTime();

    // Render loop
    loop = [&] {
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

        // glClearColor(0.f, 0.f, 0.f, 0.f);
        // glClear(GL_COLOR_BUFFER_BIT);
        double renderStart = (float) glfwGetTime();
        render();
        double renderEnd = glfwGetTime();
        cout << "Render time: " << renderEnd - renderStart << endl;
        // Render();
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

    
    currentTime = glfwGetTime();
    if (currentTime > initTime)
    {
        printf("%2.2f frames per second\n",
               ((double)frames * 1000) / (currentTime - initTime));
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}