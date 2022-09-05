#ifndef MYCAMERA_H
#define MYCAMERA_H
#include "glm/glm/ext.hpp"
#include "vec3d.h"
class MyCamera {
    public:
        glm::vec3 pos;
        glm::vec3 target_pos;
        glm::vec3 angle;
        glm::mat4x4 view_mat;
        float viewport_width;
        float viewport_height;
        MyCamera(int vh, int vw);
        
        void update_view_mat(float dx, float dy);
        glm::mat4x4 get_view_mat();
        glm::vec3 right_vector();
        glm::vec3 view_dir();
        glm::vec3 up_vector();
        void set_viewport(int width, int height);

};
#endif