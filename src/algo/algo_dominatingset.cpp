// See header for general documentation

#include "algo_dominatingset.h"
#include <cmath>
#include "../utils/adjlist.h"
#include "../utils/heap.h"

using namespace std;
// un nœud déjà couvert peut quand même appartenir au DS
vector<ul> algo_dominatingset_slow(const Adjlist &g) {
  Bheap h(g.n);

  Debug("Undirected dominating set")
	for (ul u = 0; u < g.n; ++u)
		h.insert(Keyvalue(u, g.n-g.get_degree(u)));

  vector<ul> cover; cover.reserve(sqrt(g.n));
  vector<ul> isCovered(g.n, false);
  ul covered = 0;
	while (h.n > 0) {
		Keyvalue kv = h.popmin();
		ul u = kv.key;
    Debug("Found "<<u)
		cover.push_back(u); // node with highest degree is added to the cover

    for (auto &v : g.neigh_iter(u)) {
      if(!h.contains(v)) continue;
      if(!isCovered[u]) h.update_increment(v); // its neighbours are covered and loose a degree

      if(isCovered[v]) continue;
      isCovered[v] = true;
      covered ++;
      for (auto &w : g.neigh_iter(v))
        h.update_increment(w); // their neighbours loose a degree
    }

    if(!isCovered[u]) {
      isCovered[u] = true;
      covered ++;
    }

    if(covered >= g.n) break;
	}
  Info("Dominating set " << cover.size())

	return cover;
}


vector<ul> algo_dominatingset(const Adjlist &g) {
  Bheap h(g.n);

  Debug("Undirected dominating set")
	for (ul u = 0; u < g.n; ++u)
		h.insert(Keyvalue(u, g.n-g.get_degree(u)));

  vector<ul> cover; cover.reserve(sqrt(g.n));
	while (h.n > 0) {
		Keyvalue kv = h.popmin();
		ul u = kv.key;
    Debug("Found "<<u)
		cover.push_back(u); // node with highest degree is added to the cover

    for (auto &v : g.neigh_iter(u)) {
      if(!h.contains(v)) continue;

      h.remove(v); // its neighbours are eliminated because already covered
      for (auto &w : g.neigh_iter(v)) {
        if(!h.contains(w)) continue;
        h.update_increment(w); // their neighbours loose a degree
      }
    }
	}
  Info("Dominating set " << cover.size())

	return cover;
}
