// See header for general documentation

#include "order_slashburn.h"
#include "../utils/adjlist.h"
#include "../utils/heap.h"

using namespace std;

void slash(const Adjlist &g, Bheap &hubs, Bheap &spokes, const ul u) {
  hubs.remove(u);
  spokes.remove(u);
  for (auto &v : g.neigh_iter(u)) { // for its neighbours v,
    hubs.update_increment(v); // v is less of a hub
    spokes.update_decrement(v); // v is more isolated
  }
}

vector<ul> order_slashburn(const Adjlist &g) {
  vector<ul> deg(g.n, 0);
  if(g.directed) {
    Info("Directed slashburn")
    for (ul u = 0; u < g.n; ++u)
      for (auto &v : g.neigh_iter(u))
        deg[v] ++; // compute in-degree
  }
  else {
    Info("Undirected slashburn")
    for (ul u = 0; u < g.n; ++u)
      deg[u] = g.get_degree(u);
  }

  Bheap hubs(g.n);
  Bheap spokes(g.n);
  for (ul u = 0; u < g.n; ++u) {
    hubs.insert(Keyvalue(u, g.n - deg[u])); // heap to get highest degrees
    spokes.insert(Keyvalue(u, deg[u])); // heap to get lowest degrees
  }

  vector<ul> rank; rank.reserve(g.n);
  ul begin = 0, end = g.n-1;
  while(begin <= end) {
    while(spokes.n > 0 and spokes.kv[0].val == 0) { // for every new spoke
      Keyvalue kv = spokes.getmin();
      ul u = kv.key;
      // if(hubs.n == 0) Alert("Empty hubs, spokes " << spokes.n)
      rank[u] = end--; // put the spoke at the end
      slash(g, hubs, spokes, u);
    }

    if(hubs.n == 0) break;
    Keyvalue kv = hubs.getmin();
    ul u = kv.key;
    // if(spokes.n == 0) Alert("Empty spokes, hubs " << hubs.n)
    rank[u] = begin++; // put the hub at the beginning
    slash(g, hubs, spokes, u);
  }

  Debug("Last insertion "<<begin <<" "<<end<<", heap: "<<hubs.n <<" "<<spokes.n)
  return rank;
}



// Input: Edge set E of a graph G = (V, E),
// a constant k (default = 1).
// Output: Array Γ containing the ordering V → [n].
// 1: Remove k-hubset from G to make the new graph G 0 . Add
// the removed k-hubset to the front of Γ.
// 2: Find connected components in G 0 . Add nodes in
// non-giant connected components to the back of Γ, in the
// decreasing order of sizes of connected components they
// belong to.
// 3: Set G to be the giant connected component (GCC) of G 0 .
// Go to step 1 and continue, until the number of nodes in
// the GCC is smaller than k
//
// vector<ul> slashburn(const Dadjlist &g, ul k) {
// 	g = copy(g);
// 	vector<ul> rank;
// 	while(gcc.size() >= k) {
// 		for(ul i=0; i<k; ++i) {
// 			ul hub = g.find_hub();
// 			rank.push_back(hub);
// 			g.remove_node(hub);
// 		}
//
// 		g2 = GCC(g); // Giant Connected Component, see Tarjan algorithm
// 		slashburn(g2);
// 		rank.push_back(what remains);
// 	}
// }
//
