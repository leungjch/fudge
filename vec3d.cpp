#include "vec3d.h"
#include <math.h>
#include <random>

Vec3D::Vec3D(double x, double y, double z) : x{x}, y{y}, z{z} {}

double Vec3D::distance(Vec3D v1, Vec3D v2)
{
    return sqrt(pow(v1.x - v2.x, 2.0) + pow(v1.y - v2.y, 2.0) + pow(v1.z - v2.z, 2.0));
}

Vec3D Vec3D::zero() {
    return Vec3D(0.0,0.0,0.0);
}

Vec3D Vec3D::random(double range) {
   double lower_bound = 0.0;
   double upper_bound = range;
   std::uniform_real_distribution<double> unif(lower_bound,upper_bound);
   std::default_random_engine re;
   double rand_x = unif(re);
   double rand_y = unif(re);
   double rand_z = unif(re);
   return Vec3D(rand_x, rand_y, rand_z); 
}