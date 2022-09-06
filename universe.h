
#include "graph.h"
#include <random>

class Universe
{
public:
  double dt;
  double repulsion;
  double spring_k;
  double damping;
  double gravity;
  int n_iterations;
  Graph graph;
  Universe(Graph graph, 
    double dt,
    double repulsion,
    double spring_k,
    double damping,
    double gravity);

  void update(double deltaT);
  Vec3D compute_spring_force(Node n1, Node n2);
  Vec3D compute_repulsion_force(Node n1, Node n2);
  Vec3D compute_spring_force_general(double k, Vec3D v1, Vec3D v2);
  void set_graph(Graph graph);
};