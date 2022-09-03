
#include "graph.h"

class Universe
{
public:
  double delta;
  int steps;
  double default_repulsion;
  double default_spring_k;
  double default_damping;
  Graph graph;
  Universe(Graph graph, 
    double delta,
    int steps,
    double default_repulsion,
    double default_spring_k,
    double default_damping);

  void update();
};