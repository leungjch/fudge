#include <math.h>
#include <iostream>
#include "universe.h"
#include "graph.h"
#ifndef __EMSCRIPTEN__
    #include <omp.h>
#endif
using namespace std;
Universe::Universe(Graph graph, 
    double dt,
    double default_repulsion,
    double default_spring_k,
    double default_damping) : 
        graph{graph},
        dt{dt},
        default_repulsion{default_repulsion},
        default_spring_k{default_spring_k},
        default_damping{default_damping},
        n_iterations{0}
    {}

  // Hooke's Law: F_spring = kx
  // Computes the force of n2 "pulling" on n1
Vec3D Universe::compute_spring_force(Node n1, Node n2) {
    return default_spring_k*(n2.pos - n1.pos);
}
Vec3D Universe::compute_spring_force_general(double k, Vec3D v1, Vec3D v2) {
    return k*(v1-v2);
}

// Coloumb's Law: F_repulsion = k (q1 * q2) / r^2
Vec3D Universe::compute_repulsion_force(Node n1, Node n2) {
    double dist = Vec3D::distance(n1.pos, n2.pos);
    return default_repulsion*(1/pow(dist,2.0))*Vec3D(
        (n1.pos.x - n2.pos.x),
        (n1.pos.y - n2.pos.y),
        (n1.pos.z - n2.pos.z)
    );
}
void Universe::set_graph(Graph graph) {
    this->graph = graph;
    this->graph.update_degrees();
    n_iterations = 0;
}

void Universe::update(double deltaT) {
    // Do Euler integration (O(n^2))
    for (int i = 0; i < this->graph.adj_list.size(); i++) {

        Node& n1 = graph.node_list[i];
        // Vec3D f_spring = Vec3D::random(10.0);
        // Vec3D f_repulsion = Vec3D::random(10.0);
        Vec3D f_spring = Vec3D::zero();
        Vec3D f_repulsion = Vec3D::zero();

        #pragma omp parallel for
        for (int j = 0; j < this->graph.adj_list.size(); j++) {
            if (i == j) {
                continue;
            }

            Node& n2 = graph.node_list[j]; 

            // Apply a repulsion force
            f_repulsion = f_repulsion + compute_repulsion_force(n1, n2);

            // Check if n1 and n2 are adjacent
            // If so, apply a spring force on both of them
            if (this->graph.adj_list[i].count(j)) {
                // Apply spring force
                f_spring = f_spring + compute_spring_force(n1, n2);
            }
            
        }
        // Optional: pull towards the origin
        f_spring = f_spring - compute_spring_force_general(0.2, n1.pos, Vec3D::zero());

        // Integrate laws of motion
        Vec3D f_net = f_spring + f_repulsion;


        // Compute acceleration
        // a = F_net/m
        Vec3D new_acc = (f_net-n1.vel*default_damping)*(1.0/n1.mass); 
        Vec3D new_vel = n1.vel + new_acc*deltaT;
        Vec3D new_pos = n1.pos + new_vel*deltaT;

        n1.pos = new_pos;
        n1.vel = new_vel;
        n1.acc = new_acc;
    }
    n_iterations += 1;
}