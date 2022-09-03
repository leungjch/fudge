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
Vec3D operator+(const Vec3D& lhs, const Vec3D& rhs) {
    return Vec3D(
                lhs.x + rhs.x,
                lhs.y + rhs.y,
                lhs.z + rhs.z
    );
}
Vec3D operator*(const double lhs, const Vec3D& rhs) {
    return Vec3D(
        lhs*rhs.x,
        lhs*rhs.y,
        lhs*rhs.z
    );
}
Vec3D operator*(const Vec3D& lhs, const double rhs) {
    return Vec3D(
        rhs*lhs.x,
        rhs*lhs.y,
        rhs*lhs.z
    );
}

Vec3D operator/(const Vec3D& lhs, const Vec3D& rhs) {
    return Vec3D(
        lhs.x/rhs.y,
        lhs.y/rhs.y,
        lhs.z/rhs.z
    );
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