// See header for general documentation

#include "algo_kcore.h"
#include "../utils/adjlist.h"
#include "../utils/heap.h"

using namespace std;

// To improve: all functions below are very similar. Find a way to spare repetitions.

Kdegeneracies algo_kcore(const Adjlist &g) {
  Bheap h(g.n);
  if(g.directed) {
    Debug("Directed in-kcore")
    vector<ul> degIn(g.n, 0);
    for (ul u = 0; u < g.n; ++u)
      for (auto &v : g.neigh_iter(u))
        degIn[v] ++;
    for (ul u = 0; u < g.n; ++u)
		  h.insert(Keyvalue(u, degIn[u]));
  }
  else {
    Debug("Undirected kcore")
  	for (ul u = 0; u < g.n; ++u)
  		h.insert(Keyvalue(u, g.get_degree(u)));
  }

  Kdegeneracies kd(g.n);
	ul degeneracy = 0;
	for (ul i = 1; i <= g.n; ++i) {
		Keyvalue kv = h.popmin();

		ul u = kv.key;
		kd.rank[u] = g.n - i;
		if(kv.val > degeneracy)	degeneracy = kv.val;
		kd.degeneracies[u] = degeneracy;
    int count = 0;
    for (auto &v : g.neigh_iter(u)) {
      if(h.contains(v)) count ++;
      h.update_decrement(v);
    }
    if(degeneracy > 7)
      Debug(i<<"th is " <<u << " with degeneracy " << degeneracy << ", val " << kv.val<<" ("<<count<<") / "<<g.get_degree(u))
      // Debug(i<<"th is " <<u << " with degeneracy " << degeneracy << ", deg " << kv.val<<" / ? c="<<count)
	}
  Info("Graph degeneracy " << degeneracy)

	return kd;
}

Kdegeneracies algo_kcore(const Badjlist &g) {
  Debug("Bidirected kcore")
	Bheap h(g.n);
	for (ul u = 0; u < g.n; ++u)
		h.insert(Keyvalue(u, g.get_deg(u)));

  Kdegeneracies kd(g.n);
	ul degeneracy = 0;
	for (ul i = 1; i <= g.n; ++i) {
		Keyvalue kv = h.popmin();

		ul u = kv.key;
		kd.rank[u] = g.n - i;
		if(kv.val > degeneracy)	degeneracy = kv.val;
		kd.degeneracies[u] = degeneracy;
    Debug(i<<"th is " <<u << " with degeneracy " << degeneracy << ", deg " << kv.val << " / "<<g.get_deg(u))
    for (auto &v : g.neighIn_iter(u))
      h.update_decrement(v);
    for (auto &v : g.neighOut_iter(u))
      h.update_decrement(v);
	}
  Info("Graph degeneracy " << degeneracy)

	return kd;
}

Kdegeneracies algo_kcoreIn(const Badjlist &g) {
  Debug("In-kcore")
	Bheap h(g.n);
	for (ul u = 0; u < g.n; ++u)
		h.insert(Keyvalue(u, g.get_degIn(u)));

  Kdegeneracies kd(g.n);
	ul degeneracy = 0;
	for (ul i = 1; i <= g.n; ++i) {
		Keyvalue kv = h.popmin();

		ul u = kv.key;
		kd.rank[u] = g.n - i;
		if(kv.val > degeneracy)	degeneracy = kv.val;
		kd.degeneracies[u] = degeneracy;
    Debug(i<<"th is " <<u << " with degeneracy " << degeneracy << ", deg " << kv.val << " / "<<g.get_degIn(u))
    for (auto &v : g.neighOut_iter(u))
      h.update_decrement(v);
	}
  Info("Graph degeneracy " << degeneracy)

	return kd;
}

Kdegeneracies algo_kcoreOut(const Badjlist &g) {
  Debug("Out-kcore")
  Bheap h(g.n);
	for (ul u = 0; u < g.n; ++u)
		h.insert(Keyvalue(u, g.get_degOut(u)));

  Kdegeneracies kd(g.n);
	ul degeneracy = 0;
	for (ul i = 1; i <= g.n; ++i) {
		Keyvalue kv = h.popmin();

		ul u = kv.key;
		kd.rank[u] = g.n - i;
		if(kv.val > degeneracy)	degeneracy = kv.val;
		kd.degeneracies[u] = degeneracy;
    Debug(i<<"th is " <<u << " with degeneracy " << degeneracy << ", deg " << kv.val << " / "<<g.get_degOut(u))
    for (auto &v : g.neighIn_iter(u))
      h.update_decrement(v);
	}
  Info("Graph degeneracy " << degeneracy)

	return kd;
}


Kdegeneracies algo_icore(const Adjlist &g) {
  Bheap h(g.n);
  if(g.directed) {
    Debug("Inverted directed core")
    vector<ul> degIn(g.n, 0);
    for (ul u = 0; u < g.n; ++u)
      for (auto &v : g.neigh_iter(u))
        degIn[v] ++;
    for (ul u = 0; u < g.n; ++u)
  		h.insert(Keyvalue(u, g.n-degIn[u]));
  }
  else {
    Debug("Inverted undirected core")
  	for (ul u = 0; u < g.n; ++u)
  		h.insert(Keyvalue(u, g.n-g.get_degree(u)));
  }

  Kdegeneracies kd(g.n);
	ul degeneracy = 0;
	for (ul i = 1; i <= g.n; ++i) {
		Keyvalue kv = h.popmin();

		ul u = kv.key;
		kd.rank[u] = g.n - i;
    if(degeneracy==0 and kv.val == g.n-1) degeneracy = g.n-u;
    Debug(i<<"th is " <<u << " with degeneracy " << degeneracy << ", deg " << g.n-kv.val << " / ?")
    for (auto &v : g.neigh_iter(u))
      h.update_increment(v);
	}
  Info("Isolates: " << degeneracy)

	return kd;
}

Kdegeneracies algo_icore(const Badjlist &g) {
  Debug("Inverted core")
	Bheap h(g.n);
	for (ul u = 0; u < g.n; ++u)
		h.insert(Keyvalue(u, g.n-g.get_deg(u)));

  Kdegeneracies kd(g.n);
	ul degeneracy = 0;
	for (ul i = 1; i <= g.n; ++i) {
		Keyvalue kv = h.popmin();

		ul u = kv.key;
		kd.rank[u] = g.n - i;
    if(degeneracy==0 and kv.val == g.n-1) degeneracy = g.n-u;
    Debug(i<<"th is " <<u << " with degeneracy " << degeneracy << ", deg " << g.n-kv.val << " / "<<g.get_deg(u))
    for (auto &v : g.neighIn_iter(u))
      h.update_increment(v);
    for (auto &v : g.neighOut_iter(u))
      h.update_increment(v);
	}
  Info("Isolates: " << degeneracy)

	return kd;
}


Kdegeneracies algo_icoreIn(const Badjlist &g) {
  Debug("Inverted in-core")
	Bheap h(g.n);
	for (ul u = 0; u < g.n; ++u)
		h.insert(Keyvalue(u, g.n - g.get_degIn(u)));

  Kdegeneracies kd(g.n);
	ul degeneracy = 0;
	for (ul i = 1; i <= g.n; ++i) {
		Keyvalue kv = h.popmin();

		ul u = kv.key;
		kd.rank[u] = g.n - i;
		if(degeneracy==0 and kv.val == g.n-1) degeneracy = g.n-u;
    Debug(i<<"th is " <<u << " with degeneracy " << degeneracy << ", deg " << g.n-kv.val << " / "<<g.get_degIn(u))
    for (auto &v : g.neighOut_iter(u))
      h.update_increment(v);
	}
  Info("Isolates: " << degeneracy)

	return kd;
}

Kdegeneracies algo_icoreOut(const Badjlist &g) {
  Debug("Inverted in-core")
	Bheap h(g.n);
	for (ul u = 0; u < g.n; ++u)
		h.insert(Keyvalue(u, g.n - g.get_degOut(u)));

  Kdegeneracies kd(g.n);
	ul degeneracy = 0;
	for (ul i = 1; i <= g.n; ++i) {
		Keyvalue kv = h.popmin();

		ul u = kv.key;
		kd.rank[u] = g.n - i;
    if(degeneracy==0 and kv.val == g.n-1) degeneracy = g.n-u;
    Debug(i<<"th is " <<u << " with degeneracy " << degeneracy << ", deg " << g.n-kv.val << " / "<<g.get_degOut(u))
    for (auto &v : g.neighIn_iter(u))
      h.update_increment(v);
	}
  Info("Isolates: " << degeneracy)

	return kd;
}


vector<ul> algo_kcore_rank(const Adjlist &g) {
	return algo_kcore(g).rank;
}

// Kdegeneracies algo_kcore(const Adjlist &g) {
//   Alert("kcore decomposition should be used with Uadjlist or Badjlist")
//   return Kdegeneracies(0);
// }
// Kdegeneracies algo_kcoreIn(const Adjlist &g) {
//   return algo_kcore(g);
// }
// Kdegeneracies algo_kcoreOut(const Adjlist &g) {
//   return algo_kcore(g);
// }
