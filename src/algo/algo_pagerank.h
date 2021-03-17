#ifndef ORDER_MINLA_H
#define ORDER_MINLA_H

// Compute PageRank with the power iteration method

#include "../utils/tools.h"

struct Edgelist;

// approximation of PageRank given restart parameter alpha and iterations
std::vector<double> algo_pagerank(Edgelist &g, int iterations=100, double alpha=.15);
// one step updates PageRank following each edge of the graph
void pagerank_step(const Edgelist &g, std::vector<double> &pr, double alpha);

#endif
