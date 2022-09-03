
#include "graph.h"

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

  void update();
};