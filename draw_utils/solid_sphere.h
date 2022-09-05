#ifndef FUDGE_SPHERE_H
#define FUDGE_SPHERE_H
#include <vector>
#include <GL/glew.h>
#include <GL/glu.h>
class SolidSphere
{

protected:
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> texcoords;
    std::vector<GLushort> indices;
    int shaderProgram;
    glm::mat4 MVP = glm::mat4(1.0f);
    unsigned int VBO, VAO;
    glm::vec3 color = glm::vec3(1,1,1);
public:
    SolidSphere(float radius, unsigned int rings, unsigned int sectors);
    void draw(GLfloat x, GLfloat y, GLfloat z);
    void setMVP(glm::mat4x4 mvp);
};
#endif