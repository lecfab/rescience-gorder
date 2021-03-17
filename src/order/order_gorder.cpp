#include <vector>

#include "order_gorder.h"
#include "../utils/edgelist.h"
#include "../utils/adjlist.h"
#include "../utils/tools.h"
#include "../utils/unitheap.h"
#include "../utils/inout.h"
#include "order_rcm.h"

using namespace std;

vector<ul> complete_gorder(Edgelist &h, ul window) {
  Info("Applying RCM order");
  vector<ul> rank_rcm = order_rcm(h);
  // c_printorder(rank_rcm, h.n, "ord_1.txt");

  Info("Converting to bidirected adjacency list");
  Badjlist g(h, rank_rcm);

  Info("Applying Gorder with window "<< window);
  vector<ul> rank_gorder = order_gorder(g, window);
  // c_printorder(rank_gorder, h.n, "ord_2.txt");

  Debug("composing Gorder with RCM")
  vector<ul> rank; rank.reserve(g.n);
  for (ul u = 0; u < g.n; u++) {
    rank[u] = rank_gorder[rank_rcm[u]];
  }
  return rank;
}



vector<ul> order_gorder(const Badjlist &g, ul window) {
  vector<ul> order; order.reserve(g.n);
  UnitHeap heap(g.n);

  vector<ul> isolates; isolates.reserve(heap.huge);
  for (ul u = 0; u < g.n; ++u) {
    // heap can deal with isolated nodes with no special intervention
    if(g.get_deg(u) == 0) {// put isolated nodes at the end
      isolates.push_back(u);
      // Debug(u << " isolated with rank " << isolates.size())
    }
    else
      heap.InsertElement(u, g.get_degIn(u));
  }
  Debug("Heapsize "<<heap.heapsize)

  heap.ReConstruct(); // heap indegree DESC sort
  Debug("Heapsize "<<heap.heapsize)

  ul hub = heap.top;
  order.push_back(hub);
  heap.DeleteElement(hub);

  move_window(g, heap, hub, hub);

  while(heap.heapsize > 0) {

    ul new_node = heap.ExtractMax();
    // Debug(new_node << " with rank " << order.size())
    if(new_node >= g.n) {
      Alert("Pushing infinity "<<new_node<<" while remain "<<heap.heapsize)
      heap.safety_check();
      exit(1);
    }
    order.push_back(new_node);

    ul old_node = new_node;
    if(order.size() > window) // remove oldest node of window
      old_node = order[order.size()-window-1];
    // else Debug("no window"<<endl)
    move_window(g, heap, new_node, old_node);
  }

  order.insert(order.end(), isolates.begin(), isolates.end());

  if(order.size()<g.n) Alert(g.n-order.size() <<" are missing ")
  // Info("Gorder done!")

  return rank_from_order(order, g.n);
}



void move_window(const Badjlist &g, UnitHeap &heap, const ul &new_node, const ul &old_node) {
  auto old_parent = g.neighIn_beg(old_node); // note that neighbours are sorted so we can
  auto new_parent = g.neighIn_beg(new_node); // compute their intersection in linear time

  if(old_node == new_node) // trick for the case where there is no old_node
    old_parent = g.neighIn_end(old_node); // put directly at the END to ignore
  else { // decrease children of old node
    if(g.get_degOut(old_node) <= heap.huge)
      for(auto child : g.neighOut_iter(old_node))
        heap.lazyIncrement(child, -locality_child(g, old_node, child));
  }

  vector<ul> tmp_old_parents, tmp_new_parents;
  while(true) { // find parents that are not in the intersection of old and new node
    int factor = -1;
    if(old_parent >= g.neighIn_end(old_node)) {
      if(new_parent >= g.neighIn_end(new_node)) break; // no parents remain
      factor = 1;
    }
    else if(new_parent < g.neighIn_end(new_node)) {
      if(*new_parent == *old_parent) { // common parents should be ignored
        old_parent++;
        new_parent++;
        continue;
      }
      if(*new_parent < *old_parent) factor = 1;
    }

    if(factor == -1) {
      if(g.get_degOut(*old_parent) <= heap.huge) tmp_old_parents.push_back(*old_parent);
      old_parent++;
    }
    else {
      if(g.get_degOut(*new_parent) <= heap.huge) tmp_new_parents.push_back(*new_parent);
      new_parent++;
    }
  }

  for(auto &parent: tmp_old_parents) { // decrease parents and siblings of old node
    heap.lazyIncrement(parent, -locality_child(g, old_node, parent));
    for (auto sibling : g.neighOut_iter(parent)) {
      if(sibling != old_node) heap.lazyIncrement(sibling, -locality_sibling(g, old_node, sibling));
    }
  }

  if(g.get_degOut(new_node) <= heap.huge) // increase children of new node
    for(auto child : g.neighOut_iter(new_node))
      heap.lazyIncrement(child, +locality_child(g, new_node, child));

  for(auto &parent: tmp_new_parents) { // increase parents and siblings of new node
    heap.lazyIncrement(parent, +locality_child(g, new_node, parent));
    for (auto sibling : g.neighOut_iter(parent)) {
      if(sibling != new_node) heap.lazyIncrement(sibling, +locality_sibling(g, new_node, sibling));
    }
  }
}
