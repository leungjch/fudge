#include "universe.h"
#include "graph.h"
Universe::Universe(Graph graph, 
    double dt,
    double default_repulsion,
    double default_spring_k,
    double default_damping) : 
        graph{graph},
        dt{dt},
        default_repulsion{default_repulsion},
        default_spring_k{default_spring_k},
        default_damping{default_damping}
    {}

void Universe::update() {
    // Do Euler integration (O(n^2))
    for (int i = 0; i < this->graph.adj_list.size(); i++) {

        Node n1 = graph.node_list[i];
        double f_spring = 0;
        double f_repulsion = 0;
        for (int j = 0; j < this->graph.adj_list.size(); j++) {
            if (i == j) {
                continue;
            }

            Node n2 = graph.node_list[j]; 

            // Check if n1 and n2 are adjacent
            // If so, apply a spring force on both of them
            if (this->graph.adj_list[i].count(j)) {
                // Apply spring force
            }
            
        }

        // Integrate laws of motion
        double f_net = f_spring + f_repulsion;



    }
}