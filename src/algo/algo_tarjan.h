#ifndef ALGO_TARJAN_H
#define ALGO_TARJAN_H

// Tarjan algorithm (https://en.wikipedia.org/wiki/Tarjan's_strongly_connected_components_algorithm)
// Compute the strongly connected components of a graph

// Method: depth-first search keeping track of loops in order to discover the components

// Complexity: time O(n+m), space O(n)

// algo_tarjan ( graph G ) without recursion
//   DFSstack, SCCstack, lowlink
//   indices (n, infinity)
//   foundSCC (n, false)
//
//   for w in V
//     if indices[w] < infinity, continue
//     push w to DFSstack
//
//     while DFSstack is not empty
//       let u be the top of DFSstack
//
//       if indices[u] is infinity
//         define indicies[u] = lowlink[u] = max(indices)+1
//         push u to SCCstack
//       else
//         lowlink[u] = min(lowlink[u], backtrack)
//
//       if u has a neighbour v
//         if indices[v] is infinity
//           push v to DFSstack
//         else if not foundSCC[v]
//           lowlink[u] = min(lowlink[u], indices[v])
//
//       else (u has no more neighbour)
//         if lowlink[u] is index[u]
//           create a new SCC
//           for each v on SCCstack until u
//             put v on the SCC
//             foundSCC[v] is true

#include "../utils/tools.h"
struct Adjlist;

std::vector<ul> algo_tarjan(const Adjlist &g, ul u0=0);

# endif
