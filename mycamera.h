#ifndef MYCAMERA_H
#define MYCAMERA_H
#include "glm/glm/ext.hpp"
#include "vec3d.h"
class MyCamera {
    public:
        Vec3D pos;
        glm::vec3 position;
        double distance;
        Vec3D angle;
        MyCamera(double distance);
};
#endif