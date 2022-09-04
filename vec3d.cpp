#include "vec3d.h"
#include <math.h>
#include <random>
#include <iostream>
#include <time.h> /* time */
using namespace std;
Vec3D::Vec3D(double x, double y, double z) : x{x}, y{y}, z{z} {}

double Vec3D::distance(Vec3D v1, Vec3D v2)
{
    double distance = sqrt(pow(v1.x - v2.x, 2.0) + pow(v1.y - v2.y, 2.0) + pow(v1.z - v2.z, 2.0));
    return distance;
}

Vec3D Vec3D::zero()
{
    return Vec3D(0.0, 0.0, 0.0);
}
Vec3D operator+(const Vec3D &lhs, const Vec3D &rhs)
{
    return Vec3D(
        lhs.x + rhs.x,
        lhs.y + rhs.y,
        lhs.z + rhs.z);
}
Vec3D operator-(const Vec3D &lhs, const Vec3D &rhs)
{
    return Vec3D(
        lhs.x - rhs.x,
        lhs.y - rhs.y,
        lhs.z - rhs.z);
}
Vec3D operator*(const double lhs, const Vec3D &rhs)
{
    return Vec3D(
        lhs * rhs.x,
        lhs * rhs.y,
        lhs * rhs.z);
}
Vec3D operator*(const Vec3D &lhs, const double rhs)
{
    return Vec3D(
        rhs * lhs.x,
        rhs * lhs.y,
        rhs * lhs.z);
}

Vec3D operator/(const Vec3D &lhs, const Vec3D &rhs)
{
    return Vec3D(
        lhs.x / rhs.x,
        lhs.y / rhs.y,
        lhs.z / rhs.z);
}

ostream &operator<<(ostream &os, const Vec3D &v)
{
    os << "(" << v.x << "," << v.y << "," << v.z << ")";
    return os;
}

Vec3D Vec3D::random(double range)
{
    srand(time(NULL));
    double lower_bound = -range;
    double upper_bound = range;
    std::random_device rd;  
    std::mt19937 gen(rd()); 
    std::uniform_real_distribution<double> unif(lower_bound, upper_bound);
    double rand_x = unif(rd);
    double rand_y = unif(rd);
    double rand_z = unif(rd);
    return Vec3D(rand_x, rand_y, rand_z);
}