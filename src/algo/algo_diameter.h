#ifndef ALGO_DIAMETER_H
#define ALGO_DIAMETER_H

// Diameter of a graph (directed or not)
// Defined as the longest shortest path between two nodes

#include "../utils/tools.h"
struct Adjlist;

// Brutal method: compute the single source shortest paths from
// many random nodes, hence approximating the diameter.
ul algo_diameter(const Adjlist &g, ul repeat);

// Other methods could avoid long computation and give more accurate results
// Perfect-precision can be obtained in time O(n^3) with Floyd-Warshall algorithm
// or in time O(n*m) with Johnson's algorithm

// For unweighted graphs (like here), BFS from each node is exact with O(nm)
#endif
