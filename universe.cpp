#include "universe.h"
#include "graph.h"
Universe::Universe(Graph graph, 
    double delta,
    int steps,
    double default_repulsion,
    double default_spring_k,
    double default_damping) : 
        graph{graph},
        delta{delta},
        steps{steps},
        default_repulsion{default_repulsion},
        default_spring_k{default_spring_k},
        default_damping{default_damping}
    {}

void Universe::update() {
    // Do Euler integration (O(n^2))
    for (int i = 0; i < this->graph.adj_list.size(); i++) {
        for (int j = 0; j < this->graph.adj_list.size(); j++) {
            if (i == j) {
                continue;
            }

            Node node_1 = graph.node_list[i];
            Node node_2 = graph.node_list[j];
            
        }
    }
}