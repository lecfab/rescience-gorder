#ifndef ALGO_DS_H
#define ALGO_DS_H

// Dominating set using greedy approximation (https://en.wikipedia.org/wiki/Set_cover_problem#Greedy_algorithm)

// Put the node of highest degree in the cover, remove its neighbours, then update the graph. Repeat.
// Gives a set of nodes covering all the graph.

// Method: use a binary heap to keep track of nodes' updated degrees.

// Note: it could be an order

#include "../utils/tools.h"
struct Adjlist;
struct Bheap;

// http://ceur-ws.org/Vol-1353/paper_16.pdf -- algo 1
std::vector<ul> algo_dominatingset_slow(const Adjlist &g);

// http://ceur-ws.org/Vol-1353/paper_16.pdf -- algo 3
std::vector<ul> algo_dominatingset(const Adjlist &g);

#endif
