#include <GL/glew.h>
#include <GL/glu.h>
#include "../glm/glm/ext.hpp"
#include <iostream>
#include <vector>
#ifndef FUDGE_LINE_H
#define FUDGE_LINE
class Line {
    int shaderProgram;
    unsigned int VBO, VAO;
    std::vector<float> vertices;
    glm::vec3 startPoint;
    glm::vec3 endPoint;
    glm::mat4 MVP = glm::mat4(1.0);
    glm::vec3 lineColor;
    public:
    int setMVP(glm::mat4 mvp);
    int setColor(glm::vec3 color);
    int draw();
    Line(glm::vec3 start, glm::vec3 end);
    ~Line();
};
#endif