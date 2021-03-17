// See header for general documentation

#include "algo_nq.h"
#include <ctime>
#include <cmath>

#include "../utils/adjlist.h"

ul algo_nq(const Adjlist &g, ul repeat) {
	//cout << "Algo nq" << endl;
	srand (time(NULL));
	ul result = 0;
	while(repeat--) {
		ul u = rand() % g.n;
		ul neigh = 0, neigh2 = 0;
		for (ul i = g.cd[u]; i < g.cd[u+1]; ++i) {
			ul v = g.adj[i];
			neigh++;
      neigh2 += g.get_degree(v); // d(v) may already be in cache if the node ordering is clever
			// neigh2 += pow(g.get_degree(v), 2); // d^2 takes a little while to compute
		}
		result += neigh2;
	}
	return result;
}
