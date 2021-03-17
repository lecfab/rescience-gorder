#include <algorithm>

#include "edgelist.h"
#include "tools.h"

using namespace std;


Edgelist::Edgelist(ul n, ul e) : n(n), e(e) {
	edges.reserve(e);
}
Edgelist::Edgelist(ifstream &file) {
	edges.reserve(NLINKS);
	n=0;
	ul u, v;
	while(file >> u >> v) {
		edges.push_back(edge(u, v));
		n = max3(n, u, v);
	}
	e = edges.size();
	n++;
}

void Edgelist::sort_edges() {
  sort(edges.begin(), edges.end(), [&](const edge a, const edge b) -> bool {
    return a.first < b.first or (a.first == b.first && a.second < b.second);
  });
}

void Edgelist::apply_rank(const vector<ul> &rank) {
  compute_degrees(true);
  for(ul i=0; i<e; i++)
    edges[i] = edge(rank[edges[i].first], rank[edges[i].second]);
  compute_degrees(true);
}

void Edgelist::to_dag() { // swap edges to obtain a directed acyclic graph
  compute_degrees();
  for(ul i=0; i<e; i++) {
    ul u = edges[i].first, v = edges[i].second;
    if(u > v) {
      edges[i] = edge(v, u);
      degOut[u] --; degIn[u] ++;
      degOut[v] ++; degIn[v] --;
    }
  }
}

void Edgelist::print_c(const char* output) const {
  FILE *file = fopen(output,"w");
	for (auto &edge : edges) fprintf(file, "%lu %lu\n", edge.first, edge.second);
	fclose(file);
}


ul Edgelist::get_degIn(const ul &u) {
  if(!deg_computed)
    compute_degrees();
  return degIn[u];
}
ul Edgelist::get_degOut(const ul &u) {
  if(!deg_computed)
    compute_degrees();
  return degOut[u];
}
ul Edgelist::get_deg(const ul &u) {
  if(!deg_computed)
    compute_degrees();
  return degIn[u] + degOut[u];
}

void Edgelist::print_some(ul a) const {
  if(a>e) a=e;
  cout << "Printing " << a << " edges" << endl;
  for(ul i=0; i<a; ++i)
    cout <<"\t"<< edges[i].first << " " << edges[i].second << endl;
}

void Edgelist::compute_degrees(bool force) {
  if(deg_computed and !force) return;
  degOut = vector<ul>(n, 0);
  degIn = vector<ul>(n, 0);
	for (auto &edge : edges) {
    ++degOut[edge.first];
    ++degIn[edge.second];
  }

  deg.reserve(n);
  for (ul u = 0; u < n; ++u) {
    deg[u] = degIn[u] + degOut[u];
  }

  deg_computed = true;
}
