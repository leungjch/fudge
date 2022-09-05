#include "mycamera.h"
#include "glm/glm/ext.hpp"

MyCamera::MyCamera(double distance) : pos{Vec3D::zero()}, angle{Vec3D::zero()}, distance{distance} {}