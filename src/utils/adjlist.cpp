/* Structures for compact adjacency lists, built from a list of edges */
#include "adjlist.h"
#include "tools.h"
#include "edgelist.h"
#include <algorithm>

using namespace std;

// --------------------------------------------------
// ----------- General Adjlist structure ------------
// --------------------------------------------------

Adjlist::Adjlist(Edgelist &g, int edge_factor/*=1*/, int node_factor/*=1*/) :
    edge_factor(edge_factor),
    node_factor(node_factor),
    directed(edge_factor == node_factor),
    n(g.n),
    e(edge_factor * g.e) {
  cd.reserve(n*node_factor + 1); // warning: .size() will not be defined
	adj.reserve(g.e*edge_factor);
  ranker_reset();
}

void Adjlist::ranker_reset() {
  ranker=[](ul u) { return u; };
}

void Adjlist::print_some(int a) const {
	cout << "# printing " << a << " edges" << endl;
	for (ul u = 0; u < n; ++u) {
    for (auto &v : neigh_iter(u)) {
			cout << u << " " << v << endl;
			if(a-- < 0) return;
		}
	}
}

void Adjlist::reorder(const vector<ul> &rank) {
	Edgelist h = to_edgelist_ranked(true, rank);
	build_from_edgelist(h, true);
}

Edgelist Adjlist::to_edgelist(bool directed/*=true*/) {
	Edgelist h(n, directed ? e / edge_factor : e);
	for (ul u = 0; u < n; ++u) {
    for (auto &v : neigh_iter(u))
			if (build_edge_condition(u, v)) // if undirected, put only half of two-sided edges
				h.edges.push_back(edge( ranker(u), ranker(v) ));
		cd[u] = 0;
	}
  h.sort_edges();
	return h;
}
Edgelist Adjlist::to_edgelist_ranked(bool directed, const vector<ul> &rank) {
	ranker = [&rank](ul u) { return rank[u]; };
	Edgelist h = to_edgelist(directed);
  ranker_reset();
  return h;
}


void Adjlist::build_from_edgelist(Edgelist &g, bool sorted/*=false*/) {
  vector<ul> deg = compute_degrees(g);
	compute_cumulated_degrees(deg);
	build_adj_array(g, deg);
	if(sorted) sort_neighbours(); // sort neighbours wrt new order
}

void Adjlist::build_from_edgelist_ranked(Edgelist &g, const vector<ul> &rank) {
  ranker = [&rank](ul u) { return rank[u]; };
  build_from_edgelist(g, true);
  ranker_reset();
}

void Adjlist::compute_cumulated_degrees(vector<ul> &deg) {
	cd[0] = 0;
	for (ul u=0; u < n*node_factor; ++u) {
		cd[u+1] = cd[u] + deg[u];
		deg[u] = 0; // https://stackoverflow.com/questions/8848575/fastest-way-to-reset-every-value-of-stdvectorint-to-0/8849789#8849789
	}
}

void Adjlist::sort_neighbours() {
	for (ul u = 0; u < n*node_factor; ++u) {
		sort(neigh_beg(u), neigh_end(u));
	}
}


// --------------------------------------------------
// ---------- Undirected Adjlist structure ----------
// --------------------------------------------------

Uadjlist::Uadjlist(Edgelist &g) : Adjlist(g, 2) { build_from_edgelist(g); } // 2*e for undirected
Uadjlist::Uadjlist(Edgelist &g, const vector<ul> &rank) : Adjlist(g, 2) { build_from_edgelist_ranked(g, rank); } // 2*e for undirected

vector<ul> Uadjlist::compute_degrees(Edgelist &g) { // warning: if input file is directed, edges may be counted twice
	vector<ul> deg;
  if(g.deg_computed) {
    Info("Transfering degrees instead of recomputing");
    deg.reserve(n);
    for (ul u = 0; u < n; u++)
      deg[ranker(u)] = g.deg[u];
  }
  else {
    deg.resize(n, 0);
  	for (auto &edge : g.edges) {
  		++deg[ranker(edge.first)];
  		++deg[ranker(edge.second)]; // if undirected
  	}
  }
	return deg;
}
void Uadjlist::build_adj_array(Edgelist &g, vector<ul> &deg) {
	for (auto &edge : g.edges) {
		ul u = ranker(edge.first);
		ul v = ranker(edge.second);
		adj[ cd[u] + deg[u]++ ] = v;
		adj[ cd[v] + deg[v]++ ] = u; // if undirected
	}
}

// --------------------------------------------------
// ---------- Directed Adjlist structure ------------
// --------------------------------------------------

Dadjlist::Dadjlist(Edgelist &g) : Adjlist(g, 1) { build_from_edgelist(g); }
Dadjlist::Dadjlist(Edgelist &g, const vector<ul> &rank) : Adjlist(g, 1) { build_from_edgelist_ranked(g, rank); }

vector<ul> Dadjlist::compute_degrees(Edgelist &g) {
	vector<ul> deg;
  if(g.deg_computed) {
    Info("Transfering degrees instead of recomputing");
    deg.reserve(n);
    for (ul u = 0; u < n; u++)
      deg[ranker(u)] = g.degOut[u];
  }
  else {
    deg.resize(n, 0);
  	for (auto &edge : g.edges)
  		++deg[ranker(edge.first)];
  }
	return deg;
}
void Dadjlist::build_adj_array(Edgelist &g, vector<ul> &deg) {
	for (auto &edge : g.edges) {
		ul u = ranker(edge.first);
		adj[ cd[u] + deg[u]++ ] = ranker(edge.second);
	}
}

// --------------------------------------------------
// --------- Both sided Adjlist structure -----------
// --------------------------------------------------


Badjlist::Badjlist(Edgelist &g) : Adjlist(g, 2, 2) { build_from_edgelist(g); }
Badjlist::Badjlist(Edgelist &g, const vector<ul> &rank) : Adjlist(g, 2, 2) { build_from_edgelist_ranked(g, rank); }

vector<ul> Badjlist::compute_degrees(Edgelist &g) {
	vector<ul> deg;
  if(g.deg_computed) {
    Info("Transfering degrees instead of recomputing");
    deg.reserve(2*n);
    for (ul u = 0; u < n; u++) {
      deg[ranker(u)] = g.degOut[u];
      deg[ranker(u) + n] = g.degIn[u];
    }
  }
  else {
    deg.resize(2*n, 0);
  	for (auto &edge : g.edges) {
  		++deg[ranker(edge.first)]; // degOut
  		++deg[ranker(edge.second) + n]; // degIn is slided by n slots in the array
  	}
  }
	return deg;
}

void Badjlist::build_adj_array(Edgelist &g, vector<ul> &deg) {
	for (auto &edge : g.edges) {
		ul u = ranker(edge.first);
		ul v = ranker(edge.second);
		adj[ cd[u] + deg[u]++ ] = v; // degOut
		adj[ cd[v+n] + deg[v+n]++ ] = u; // degIn
	}
}

void Badjlist::print_some(int a) const {
	cout << "# printing " << a << " edges" << endl;
	for (ul u = 0; u < n; ++u) {
    for (auto &v : neighOut_iter(u)) {
			cout << u << " -> " << v << endl;
			if(a-- < 0) return;
		}

    for (auto &v : neighIn_iter(u)) {
			cout << u << " <- " << v << endl;
			if(a-- < 0) return;
		}
	}
}
