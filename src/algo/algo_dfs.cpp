// See header for general documentation

#include "algo_dfs.h"
#include "../utils/adjlist.h"

using namespace std;


vector<ul> algo_dfs(const Adjlist &g, ul u0) {
  Debug("Algo dfs from node "<< u0)
  vector<bool> visited(g.n, false);
  vector<Nodeneigh> stack;
  stack.reserve(g.n);
  vector<ul> rank;
  rank.reserve(g.n);

  ul r = 0;

  for (ul c = 0; c < g.n; ++c) {
    ul u = (c + u0) % g.n; // allows to start from any node u0
    if (visited[u])
      continue;

    rank[u] = r++;
    visited[u] = true;
    stack.push_back(Nodeneigh(g, u));

    while (!stack.empty()) {
      Nodeneigh &deepest = stack.back();
      if (deepest.neigh >= g.neigh_end(deepest.node))
        stack.pop_back();
      else {
        ul v = *deepest.neigh;
        deepest.neigh++;
        if (visited[v])
          continue;
        rank[v] = r++;
        visited[v] = true;
        stack.push_back(Nodeneigh(g, v));
      }
    }
  }

  return rank;
}
