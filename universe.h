
#include "graph.h"
#include <random>

class Universe
{
public:
  double dt;
  double default_repulsion;
  double default_spring_k;
  double default_damping;
  Graph graph;
  Universe(Graph graph, 
    double dt,
    double default_repulsion,
    double default_spring_k,
    double default_damping);

  void update(double deltaT);
  Vec3D compute_spring_force(Node n1, Node n2);
  Vec3D compute_repulsion_force(Node n1, Node n2);
  void set_graph(Graph graph);
};