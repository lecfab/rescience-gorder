// See header for general documentation

#include <vector>
#include "algo_bfs.h"
#include "../utils/adjlist.h"
#include "../utils/tools.h"

using namespace std;


vector<ul> algo_bfs(const Adjlist &g, ul u0) {
  Debug("Algo bfs from node " << u0)
  vector<bool> placed(g.n, false);
  vector<ul> order;
  order.reserve(g.n); // warning: this is an inverted rank
  Debug("Algo bfs from node " << u0)
  ul i = 0;
  for (ul c = 0; c < g.n; ++c) {
    ul u = (c + u0) % g.n; // allows to start from any node u0
    if (placed[u])
      continue;

    order.push_back(u);
    placed[u] = true;

    while (i < order.size()) {
      ul w = order[i++];
      for (auto &v : g.neigh_iter(w)) {
        if (placed[v])
          continue;
        placed[v] = true;
        order.push_back(v);
      }
    }
  }Debug("Algo bfs from node " << u0)
  if(g.n != order.size()) Alert("BFS found only "<<order.size()<<" nodes out of "<<g.n)

  return order;
}
