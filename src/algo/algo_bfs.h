#ifndef ALGO_BFS_H
#define ALGO_BFS_H

// Breadth-first search in a graph (directed or not)
// Complexity: time O(n+m), size O(n)

#include <vector>
#include "../utils/tools.h"
struct Adjlist;

// Main function: returns an order
std::vector<ul> algo_bfs(const Adjlist &g, ul u0=0);


#endif
