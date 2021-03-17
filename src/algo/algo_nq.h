#ifndef ALGO_NQ_H
#define ALGO_NQ_H

// Neighbour Query
// To see the benefit of ordering nodes, compute a useless value over some
// node's neighbours, and compare the speed throughout various orders.

#include "../utils/tools.h"
struct Adjlist;

// Take a random node and compute the sum degrees of its neighbours
ul algo_nq(const Adjlist &g, ul repeat);

#endif
