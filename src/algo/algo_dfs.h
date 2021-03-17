#ifndef ALGO_DFS_H
#define ALGO_DFS_H

// Depth-first search in a graph (directed or not)

// Procedure DFS_iterative(G, v) is
//     let S be a stack
//     S.push(iterator of G.adjacentEdges(v))
//     while S is not empty do
//         if S.peek().hasNext() then
//             w = S.peek().next()
//             if w is not labeled as discovered then
//                 label w as discovered
//                 S.push(iterator of G.adjacentEdges(w))
//         else
//             S.pop()

// Complexity: time O(n+m), size O(n)
#include <vector>
#include "../utils/tools.h"
#include "../utils/adjlist.h" // struct Adjlist;


// Main function: returns a ranking
std::vector<ul> algo_dfs(const Adjlist &g, ul u0=0);

// Structure to store a node with a pointer to its next neighbour
struct Nodeneigh {
  const ul node;
  const ul *neigh;
  Nodeneigh(const Adjlist &g, ul &u) : node(u), neigh(g.neigh_beg(u)) {}
};

#endif
