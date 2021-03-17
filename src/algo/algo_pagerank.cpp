// See header for general documentation

#include "algo_pagerank.h"
#include "../utils/edgelist.h"

using namespace std;


vector<double> algo_pagerank(Edgelist &g, int iterations, double alpha) {
  g.compute_degrees();
  vector<double> pr(g.n, 1./g.n);

  for(int i=0; i<iterations; ++i)
    pagerank_step(g, pr, alpha);

  return pr;
}

void pagerank_step(const Edgelist &g, vector<double> &pr, double alpha) {
  for(ul u=0; u < g.n; ++u) { // how much each node will give to its neighbours
    double deg_u = g.degOut[u];
    if(deg_u > 0) pr[u] /= deg_u;
  }

  vector<double> updated_pr(g.n, 0);
  for (auto &e : g.edges) // walkers are transfered along edges
    updated_pr[e.second] += pr[e.first];

  const double uniform = 1./g.n;
  double sum = 0.;
  for(ul u=0; u < g.n; ++u) { // restart rate is applied
    pr[u] = alpha*uniform + (1.-alpha)*updated_pr[u];
    sum += pr[u];
  }
  double leftover = (1.-sum) * uniform; // distribute leftover to sum to one
  for(ul u=0; u < g.n; ++u) pr[u] += leftover;
}

// void printres(FILE* file,unsigned long long n, double* vect){
// 	for (ul i=0;i<n;i++) fprintf(file,"%llu %10le\n",i,vect[i]);
// }
