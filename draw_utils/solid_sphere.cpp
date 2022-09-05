#include <vector>
#include <GL/glew.h>
#include <GL/glu.h>
#include <iostream>
#include "../glm/glm/ext.hpp"
#include "solid_sphere.h"
// Utility class for drawing a sphere
// Adapted to modern OpenGL from https://stackoverflow.com/questions/5988686/creating-a-3d-sphere-in-opengl-using-visual-c
SolidSphere::SolidSphere(float radius, unsigned int rings, unsigned int sectors)
{
    radius = radius; 
}

void SolidSphere::init() {


    const char *vertexShaderSource = "#version 300 es \n"
                                     "layout (location = 0) in vec3 aPos;\n"
                                     "uniform mat4 model;\n"
                                     "uniform mat4 view;\n"
                                     "uniform mat4 projection;\n"
                                     "void main()\n"
                                     "{\n"
                                     "   gl_Position = projection * view * model * vec4(aPos, 1.0f);\n"
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
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    };
    // fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    // check for shader compile errors
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    };

    // link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    GLfloat x, y, z, alpha, beta; // Storage for coordinates and angles        
    GLfloat radius = 1.0f;
    int gradation = 20;
    vertices.clear();
    for (alpha = 0.0; alpha < M_PI; alpha += M_PI/gradation)
    {        
        for (beta = 0.0; beta < 2.01*M_PI; beta += M_PI/gradation)            
        {            
            x = radius*cos(beta)*sin(alpha);
            y = radius*sin(beta)*sin(alpha);
            z = radius*cos(alpha);
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
            x = radius*cos(beta)*sin(alpha + M_PI/gradation);
            y = radius*sin(beta)*sin(alpha + M_PI/gradation);
            z = radius*cos(alpha + M_PI/gradation);            
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }        
    }


//     vertices = {
//     -1.0f,-1.0f,-1.0f, // triangle 1 : begin
//     -1.0f,-1.0f, 1.0f,
//     -1.0f, 1.0f, 1.0f, // triangle 1 : end
//     1.0f, 1.0f,-1.0f, // triangle 2 : begin
//     -1.0f,-1.0f,-1.0f,
//     -1.0f, 1.0f,-1.0f, // triangle 2 : end
//     1.0f,-1.0f, 1.0f,
//     -1.0f,-1.0f,-1.0f,
//     1.0f,-1.0f,-1.0f,
//     1.0f, 1.0f,-1.0f,
//     1.0f,-1.0f,-1.0f,
//     -1.0f,-1.0f,-1.0f,
//     -1.0f,-1.0f,-1.0f,
//     -1.0f, 1.0f, 1.0f,
//     -1.0f, 1.0f,-1.0f,
//     1.0f,-1.0f, 1.0f,
//     -1.0f,-1.0f, 1.0f,
//     -1.0f,-1.0f,-1.0f,
//     -1.0f, 1.0f, 1.0f,
//     -1.0f,-1.0f, 1.0f,
//     1.0f,-1.0f, 1.0f,
//     1.0f, 1.0f, 1.0f,
//     1.0f,-1.0f,-1.0f,
//     1.0f, 1.0f,-1.0f,
//     1.0f,-1.0f,-1.0f,
//     1.0f, 1.0f, 1.0f,
//     1.0f,-1.0f, 1.0f,
//     1.0f, 1.0f, 1.0f,
//     1.0f, 1.0f,-1.0f,
//     -1.0f, 1.0f,-1.0f,
//     1.0f, 1.0f, 1.0f,
//     -1.0f, 1.0f,-1.0f,
//     -1.0f, 1.0f, 1.0f,
//     1.0f, 1.0f, 1.0f,
//     -1.0f, 1.0f, 1.0f,
//     1.0f,-1.0f, 1.0f
// };



    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glVertexAttribPointer(
        0,                                // attribute. No particular reason for 1, but must match the layout in the shader.
        3,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
void SolidSphere::setMVP(glm::mat4 m, glm::mat4 v, glm::mat4 p)
{
   model = m;
   view = v;
   projection = p; 
}

void SolidSphere::draw()
{
    // glMatrixMode(GL_MODELVIEW);
    // glPushMatrix();
    // glTranslatef(x, y, z);

    // glEnableClientState(GL_VERTEX_ARRAY);
    // glEnableClientState(GL_NORMAL_ARRAY);
    // glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    // glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
    // glNormalPointer(GL_FLOAT, 0, &normals[0]);
    // glTexCoordPointer(2, GL_FLOAT, 0, &texcoords[0]);
    // glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_SHORT, &indices[0]);
    // glPopMatrix();

    glUseProgram(shaderProgram);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);
    glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1, &color[0]);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size());

}