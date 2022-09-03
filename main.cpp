#include <iostream>
#include "graph.h"
#include "universe.h"
using namespace std;
int main() {
    cout << "Hello world!" << endl;
    Graph graph;

    int n1 = graph.add_node("A");
    int n2 = graph.add_node("B");
    graph.add_edge(n1,n2);

    Universe universe(graph, 0.1, 100, 1.0, 1.0, 1.0 );
    return 0;
}