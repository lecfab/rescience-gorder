#ifndef ORDER_GORDER_H
#define ORDER_GORDER_H

// Hao Wei, Jeffrey Xu Yu, Can Lu, and Xuemin Lin.
// Speedup Graph Processing by Graph Ordering.
// In Proceedings of SIGMOD '16, 2016.

#include <cmath>
#include "../utils/tools.h"

struct Edgelist;
struct Badjlist;
struct UnitHeap;

std::vector<ul> complete_gorder(Edgelist &h, ul window = 5);
std::vector<ul> order_gorder(const Badjlist &g, ul window = 5);
void move_window(const Badjlist &g, UnitHeap &heap,
                 const ul &new_node, const ul &old_node);

inline int locality_parent(const Badjlist &g, const ul &u, const ul &v) {
  return 1;
}
inline int locality_child(const Badjlist &g, const ul &u, const ul &v) {
  return 1;
}
inline int locality_sibling(const Badjlist &g, const ul &u, const ul &v) {
  return 1; // ceil(10*2 / (g.get_degIn(u) + g.get_degIn(v)));
}

#endif
