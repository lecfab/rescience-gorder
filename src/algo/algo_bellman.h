#ifndef ALGO_BELLMAN_H
#define ALGO_BELLMAN_H


// Bellman-Ford algorithm (https://en.wikipedia.org/wiki/Bellman-Ford_algorithm)
// Compute the shortest paths from a source vertex to any other vertex
// Note that BFS gives the same results in an unweighted graph

// Procedure Bellman_Ford(G, s)
//   for each node u in G
//   |        d[u] = +∞
//   d[s] = 0
//   for k between 1 and |V(G)| do
//    |    for each edge (u, t) in E(G) do
//    |      |    d[t] := min(d[t], d[u] + 1)
//   return distances d

// Optimisations:
// * Use edge (v,w) only if v has been discovered
// * Use edge (v,w) only if v has been updated to a smaller distance
// * Stop when no node is updated

// Complexity: time O(m*a) where a≤diameter is the longest path starting from u

#include <vector>
#include "../utils/tools.h"
struct Adjlist;

// Run the procedure with source vertex u
std::vector<ul> algo_bellman(const Adjlist &g, const ul u);
std::vector<ul> algo_bellman_complete(const Adjlist &g, const ul u0);

#endif
