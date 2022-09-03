#ifndef NODE_H
#define NODE_H
#include <string>
#include "vec3d.h"

using namespace std;

class Node {
    public:
        int id;
        string data;

        // Physical properties
        Vec3D pos;
        Vec3D vel;
        Vec3D acc;
        int radius;
        double repulsion;
        double stiffness;
        double damping;

        Node(int _id, string _data);
};
#endif