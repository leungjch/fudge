#ifndef GRAPH_H
#define GRAPH_H
#include <vector>
#include <string>
#include <unordered_set>
#include "node.h"
using namespace std;

// Adjacency list representation of a graph
class Graph {
    public:

        Graph();

        // A vector of neighbor nodes for each node id
        // e.g. adj_list[0] represents the neighbors of node 0
        vector<unordered_set<int>> adj_list;
        vector<Node> node_list;
        
        // Add a new node to the graph
        // Returns the id of the newly added node
        int add_node(string data);

        // Add an edge connecting node_1 and node_2
        void add_edge(int node_id_1, int node_id_2);

        // Update the degree of each node
        void update_degrees();
};
#endif