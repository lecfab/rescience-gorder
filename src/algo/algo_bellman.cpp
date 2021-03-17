// See header for general documentation

#include "algo_bellman.h"
#include "../utils/tools.h"
#include "../utils/adjlist.h"

using namespace std;


vector<ul> algo_bellman(const Adjlist &g, const ul u) {
  Debug("Algo bellman from node "<< u)
	vector<ul> dist(g.n, g.n+2); // initialised to infinite distance
	dist[u] = 0;
	vector<ul> updates; updates.reserve(g.n);
	updates.push_back(u);

	for (ul k = 0; k < g.n; ++k) { // stops after Diameter loops
		ul j = 0;
		while(j < updates.size()) { // only check nodes whose distance has been updated
			const ul *v = &updates[j ++];

			for (const ul *w = g.neigh_beg(*v); w < g.neigh_end(*v); ++w) { // for each outgoing edge
				if(dist[*w] > dist[*v]+1) {
					dist[*w] = dist[*v]+1;
					updates.push_back(*w); // tag w as updated
				}
			}
		}
		if(updates.size() == 0) break; // stop if no node has been updated
		updates.clear();
	}

	return dist;
}

vector<ul> algo_bellman_complete(const Adjlist &g, const ul u0) {
  Debug("Algo bellman complete from node "<< u0)
	vector<ul> dist(g.n, g.n+2); // initialised to infinite distance
  vector<ul> sources; sources.reserve(g.n / 5);

  vector<ul> updates; updates.reserve(g.n);
  ul found = 0;
  for(ul c=0; c<g.n; ++c) { // ensures every node is seen
    ul u = (u0 + c*101) % g.n;
    if(dist[u] < g.n) continue;
  	dist[u] = 0;
    sources.push_back(u);
  	updates.push_back(u);
    found ++;

  	for (ul k = 0; k < g.n; ++k) { // stops after Diameter loops
  		ul j = 0;
  		while(j < updates.size()) { // only check nodes whose distance has been updated
  			const ul v = updates[j ++];
  			for (const auto &w : g.neigh_iter(v)) { // for each outgoing edge
          if(dist[w] >= g.n) found ++;
  				if(dist[w] > dist[v]+1) {
  					dist[w] = dist[v]+1;
  					updates.push_back(w); // tag w as updated
  				}
  			}
  		}
  		if(updates.size() == 0) break; // stop if no node has been updated
  		updates.clear();
  	}

    if(found > g.n / 2) break; // likely to have found the biggest component
  }
  Debug("Sources needed: "<< sources.size() << " to discover "<< 100*found/g.n <<"%")

	return dist;
}
