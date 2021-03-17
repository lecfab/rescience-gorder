// See header for general documentation

#include "algo_tarjan.h"
#include "../utils/adjlist.h"
#include "algo_dfs.h"

using namespace std;

// To improve: try to merge several vectors into one
// To check: starting from u0!=0 does not affect the result

vector<ul> algo_tarjan(const Adjlist &g, ul u0) {
  vector<Nodeneigh> DFSstack; DFSstack.reserve(g.n);
  vector<ul> SCCstack; SCCstack.reserve(g.n);
  vector<ul> indices(g.n, g.n + 2);
  vector<ul> lowlink; lowlink.reserve(g.n);
  vector<bool> foundSCC(g.n); foundSCC.reserve(g.n);
  vector<ul> SCCsizes;

  ul index = 0;
  ul backtrack = g.n + 2;

  for(ul c=0; c<g.n; ++c) { // loop to find several components
    ul root = (c + u0) % g.n; // allows to start from any node u0
    if(indices[root] < g.n) continue; // u was already found
    Debug("Start new exploration from " << root)
    DFSstack.push_back(Nodeneigh(g, root));

    while (!DFSstack.empty()) { // take u in a DFS order
      Nodeneigh &deepest = DFSstack.back();
      ul u = deepest.node;
      if(indices[u] > g.n) { // node seen for the first time, added to the SCC stack
        Debug("Discover " << u << ", index " << index)
        indices[u] = index++;
        lowlink[u] = indices[u];
        backtrack = indices[u];
        SCCstack.push_back(u);
        foundSCC[u] = false;
      }
      else { // node seen after seing its neighbours, update accordingly
        Debug("Meet again " << u << ", lowlink (" << lowlink[u] << " vs " << backtrack << ")")
        lowlink[u] = min(lowlink[u], backtrack);
      }

      if(deepest.neigh >= g.neigh_end(u)) { // u has no more neighbour
        Debug("\tNo more neighbour, backtracks " << lowlink[u])
        backtrack = lowlink[u];

        if(lowlink[u] == indices[u]) {
          Debug("\tCreate SCC " << SCCsizes.size())
          ul size = 0, v;
          do {
            v = SCCstack.back();
            Debug("\t\t" << v << " added to SCC " << indices[v])
            indices[v] = SCCsizes.size();
            foundSCC[v] = true;
            size ++;
            SCCstack.pop_back();
          } while(v != u);
          SCCsizes.push_back(size);
        }

        DFSstack.pop_back();
      }
      else { // consider next neighbour v of u
        ul v = *deepest.neigh;
        deepest.neigh ++;

        if(indices[v] > g.n) { // this neighbour has never been seen
            Debug("\tStack " << v)
            DFSstack.push_back(Nodeneigh(g, v));
        }
        else if(!foundSCC[v]) { // this neighbour has no SCC yet
          Debug("\tneighbour " << v <<" of index "<< indices[v] << " vs lowlink " << lowlink[u] << " (" << lowlink[v] <<")")
          lowlink[u] = min(lowlink[u], indices[v]);
        }
        else { // ignore v
          Debug("\tFound neighbour " << v << " with SCC " << indices[v])
        }
      }
    }
  }

  ul biggestSize = 0;
  for (size_t i = 0; i < SCCsizes.size(); i++)
    if(SCCsizes[i] > biggestSize)
      biggestSize = SCCsizes[i];
  Info("Biggest strongly connected component has size " << biggestSize << " out of " << g.n)

  return indices;
}
