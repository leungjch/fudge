#include "mycamera.h"
#include "glm/glm/ext.hpp"

MyCamera::MyCamera(int vh, int vw) : pos{glm::vec3(0, 0, 10.0)}, target_pos{glm::vec3(0, 0, 0)}, angle{glm::vec3(0, 0, 0)}, view_mat{glm::mat4x4()} {
    view_mat = get_view_mat();
    set_viewport(vw, vh);
}

glm::vec3 MyCamera::right_vector()
{
    return glm::transpose(view_mat)[0];
}
glm::vec3 MyCamera::up_vector()
{
    return glm::vec3(0.0, 1.0, 0.0);
}

glm::mat4x4 MyCamera::get_view_mat()
{
    return glm::lookAt(pos, target_pos, up_vector());
}
glm::vec3 MyCamera::view_dir() 
{
    return -glm::transpose(view_mat)[2]; 
}
void MyCamera::set_viewport(int width, int height) {
    viewport_width = (float)width;
    viewport_height = (float)height;
}

// From example here: https://asliceofrendering.com/camera/2019/11/30/ArcballCamera/
void MyCamera::update_view_mat(float dx, float dy)
{
    dx = dx * (2 * M_PI / viewport_width);
    dy = dy * ( M_PI / viewport_height);
    glm::vec4 pos4(pos.x, pos.y, pos.z, 1.0);
    glm::vec4 target_pos4(target_pos.x, target_pos.y, target_pos.z, 1.0);

    float cosAngle = glm::dot(view_dir(), up_vector());
    if (cosAngle * ((dy > 0) ? 1 : ((dy < 0) ? -1 : 0)) > 0.99f)
        dy = 0;

    // Rotate around target point on first axis
    glm::mat4x4 rotationMatrixX(1.0f);
    rotationMatrixX = glm::rotate(rotationMatrixX, dx, up_vector());
    pos4 = (rotationMatrixX * (pos4 - target_pos4)) + target_pos4;
    glm::mat4x4 rotationMatrixY(1.0f);
    
    // Rotate around target point on second axis
    rotationMatrixY = glm::rotate(rotationMatrixY, dy, right_vector());
    glm::vec4 final_pos4 = (rotationMatrixY * (pos4 - target_pos4)) + target_pos4;
    pos = glm::vec3(final_pos4[0], final_pos4[1], final_pos4[2]);

    view_mat = glm::lookAt(pos, target_pos, up_vector());
}   