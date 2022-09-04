#ifndef MYCAMERA_H
#define MYCAMERA_H
#include "vec3d.h"
class MyCamera {
    public:
        Vec3D pos;
        double angle;
        MyCamera(Vec3D initPos);
};
#endif