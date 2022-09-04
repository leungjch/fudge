#include <stdlib.h>
#include <GL/glew.h>
#include <iostream>
#include "graph.h"
#include "universe.h"
#include "vec3d.h"
#include "mycamera.h"
#include <GL/glut.h>
#include <time.h> /* time */
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <assert.h>
#include <omp.h>

using namespace std;
bool movingUp = false;       // Whether or not we are moving up or down
float yLocation = 0.0f;      // Keep track of our position on the y axis.
int n_iterations = 200;

Graph graph;
MyCamera camera(Vec3D(0,0,25));

Universe universe(graph,
                  0.05, // dt
                  1.0,   // repulsion
                  1.0,  // spring
                  1.0   // damping
);
// --------------------------------------------------------------------------------------------



void init_graph() {
    int n1 = graph.add_node("A");
    int n2 = graph.add_node("B");
    for (int i = 0; i < 1000; i++)
    {
        srand(time(NULL));
        int n3 = graph.add_node("C");
        cout << "n3 is" << n3 << " " << graph.node_list.size() << endl;
        int rd  = rand()%(graph.node_list.size());
        std::cout << "Hello " << rd << endl;
        graph.add_edge(n3,rd);

    }



    // graph.node_list[n1].pos = Vec3D(-1.0, 0, 0);
    // graph.node_list[n2].pos = Vec3D(1.0, 0, 0);
    graph.add_edge(n1, n2);

    universe.set_graph(graph);
}


void draw_graph(float yloc)
{
    static int n_nodes = universe.graph.adj_list.size();

    for (int i = 0; i < n_nodes; i++)
    { // Translate balls towards and away from front plane.
        Node nd = universe.graph.node_list[i];
        // glPushMatrix();
        // glTranslatef(balls[b][0], balls[b][1], balls[b][2]);
        // glutSolidSphere(0.2, 16, 10);
        // glPopMatrix();

        glPointSize(10);
        glBegin(GL_POINTS);
        glVertex3f(nd.pos.x, nd.pos.y, nd.pos.z);
        glEnd();
    }

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
}
void render(void)
{

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Clear the background of our window to red
    glClear(GL_COLOR_BUFFER_BIT);         // Clear the colour buffer (more buffers later on)
    glLoadIdentity();                     // Load the Identity Matrix to reset our drawing locations
    
    // Set the camera
    gluLookAt(camera.pos.x, 0.0f, camera.pos.z, camera.pos.x, 0.0f, camera.pos.z-1.0f, 0.0f, 1.0f, 0.0f);
    glRotatef(camera.angle, 1, 0, 0);
    camera.angle += 0.25f;

    draw_graph(yLocation);
    glTranslatef(0.0f, 0.0f, yLocation); // Translate our object along the y axis
}

typedef int32_t i32;
typedef uint32_t u32;
typedef int32_t b32;

#define WIN_WIDTH 1000
#define WIN_HEIGHT 1000

int main(int ArgCount, char **Args)
{

    // Test if openMP is working
    #pragma omp parallel for
    for (int i=0;i<16;++i)
    {
        cout << omp_get_max_threads() << endl;
        printf( "Thread %d works with idx %d\n", omp_get_thread_num(), i);
    }

    init_graph();

    u32 WindowFlags = SDL_WINDOW_OPENGL;
    SDL_Window *Window = SDL_CreateWindow("Force graph", 0, 0, WIN_WIDTH, WIN_HEIGHT, WindowFlags);
    assert(Window);
    SDL_GLContext Context = SDL_GL_CreateContext(Window);
    SDL_GL_SetSwapInterval(1);

    /* Set rendering settings */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (GLfloat)WIN_WIDTH / (GLfloat)WIN_HEIGHT, 1.0, 400.0); // Set the Field of view angle (in degrees), the aspect ratio of our window, and the new and far planes
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    b32 running = 1;
    b32 fullScreen = 0;

    double timeDelta = 1000/60;
    double timeAccumulator = 0;
    double timeSimulatedThisIteration = 0;
    u32 frames = 0;
    u32 currentTime = SDL_GetTicks();
    u32 startTime = SDL_GetTicks();
    u32 initTime = SDL_GetTicks();

    while (running)
    {
        timeSimulatedThisIteration = 0;
        startTime = SDL_GetTicks();
        ++frames;
        // Run update logic
        if (frames < n_iterations) {
            universe.update(1.0/timeDelta);
        }

        // while (timeAccumulator >= timeDelta) {
        //     universe.update(1.0/timeDelta);
        //     timeAccumulator -= timeDelta;
        //     timeSimulatedThisIteration += timeDelta;
        // }
        currentTime = SDL_GetTicks();
        cout << "Update time: " << currentTime - startTime << endl;

        // Handle user input
        SDL_Event Event;
        while (SDL_PollEvent(&Event))
        {
            // if (Event.type == SDL_MOUSEMOTION)
            // {
            //     cout << "Moved mouse " << Event.motion.x << endl;
            //     cout << "Moved mouse " << Event.motion.y << endl;
            // }
            if (Event.type == SDL_KEYDOWN)
            {
                switch (Event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    running = 0;
                    break;
                case 'w':
                    
                case 'f':
                    fullScreen = !fullScreen;
                    if (fullScreen)
                    {
                        SDL_SetWindowFullscreen(Window, WindowFlags | SDL_WINDOW_FULLSCREEN_DESKTOP);
                    }
                    else
                    {
                        SDL_SetWindowFullscreen(Window, WindowFlags);
                    }
                    break;
                default:
                    break;
                }
            }
            else if (Event.type == SDL_QUIT)
            {
                running = 0;
            }
        }



        glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
        // glClearColor(0.f, 0.f, 0.f, 0.f);
        // glClear(GL_COLOR_BUFFER_BIT);
        u32 renderStart = SDL_GetTicks();
        render();
        u32 renderEnd = SDL_GetTicks();
        cout << "Render time: " << renderEnd - renderStart << endl;
        // Render();
        SDL_GL_SwapWindow(Window);
        currentTime = SDL_GetTicks();
        timeAccumulator += currentTime - startTime;

    }

    currentTime = SDL_GetTicks();
    if (currentTime > initTime) {
            printf("%2.2f frames per second\n",
               ((double) frames * 1000) / (currentTime - initTime));
    }
    return 0;
}
// // --------------------------------------------------------------------------------------------

// void reshape(int width, int height)
// {
//     glViewport(0, 0, (GLsizei)width, (GLsizei)height);                // Set our viewport to the size of our window
//     glMatrixMode(GL_PROJECTION);                                      // Switch to the projection matrix so that we can manipulate how our scene is viewed
//     glLoadIdentity();                                                 // Reset the projection matrix to the identity matrix so that we don't get any artifacts (cleaning up)
//     gluPerspective(60, (GLfloat)width / (GLfloat)height, 1.0, 200.0); // Set the Field of view angle (in degrees), the aspect ratio of our window, and the new and far planes
//     glMatrixMode(GL_MODELVIEW);                                       // Switch back to the model view matrix, so that we can start drawing shapes correctly
// }

// // --------------------------------------------------------------------------------------------

// int main(int argc, char **argv)
// {

//     int n1 = graph.add_node("A");
//     int n2 = graph.add_node("B");
//     for (int i = 0; i < 500; i++)
//     {
//         int n3 = graph.add_node("C");

//         graph.add_edge(n3, rand()%graph.node_list.size());

//     }
//     // graph.node_list[n1].pos = Vec3D(-1.0, 0, 0);
//     // graph.node_list[n2].pos = Vec3D(1.0, 0, 0);
//     graph.add_edge(n1, n2);

//     universe.set_graph(graph);

//     srand(time(NULL));
//     glutInit(&argc, argv);            // Initialize GLUT
//     glutInitDisplayMode(GLUT_DOUBLE); // Set up a basic display buffer (only single buffered for now)
//     glutInitWindowSize(1000, 1000);   // Set the width and height of the window
//     glutInitWindowPosition(800, 200); // Set the position of the window
//     glutCreateWindow("Force graph");  // Set the title for the window

//     glutDisplayFunc(display); // Tell GLUT to use the method "display" for rendering
//     glutIdleFunc(display);    // Tell GLUT to use the method "display" as our idle method as well
//     glutReshapeFunc(reshape); // Tell GLUT to use the method "reshape" for reshaping

//     glutMainLoop(); // Enter GLUT's main loop

//     // cout << "Hello world!" << endl;

//     // for (int i = 0; i < 5000; i++)
//     // {
//     //     universe.update();
//     //     cout << Vec3D::distance(universe.graph.node_list[0].pos, universe.graph.node_list[1].pos) << endl;
//     // }
//     return 0;
// }
