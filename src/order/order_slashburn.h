#ifndef ORDER_SLASHBURN_H
#define ORDER_SLASHBURN_H

// SlashBurn: Graph Compression and Mining beyond Caveman Communities.
// Yongsub Lim, U Kang, and Christos Faloutsos.
// https://datalab.snu.ac.kr/~ukang/

// Define parameter k:
// 	k=1 is best but slower to precompute
// 	k=n/200 is recommanded
// Loop:
// 	Remove k-hubest
// 	Order hubs first, giant connected component then, spokes finally
// Removing:
// 	Take highest degree node
// 	Recompute degrees
// Order of spokes:
// 	Instead of taking their component size, take the size of their former hub

// This implementation, using max-degree and min-degree binary heaps:
//  Put spokes at the end of the rank
//  Put the biggest hub at the beginning
//  Repeat until all nodes have been placed

#include "../utils/tools.h"

struct Adjlist;
struct Bheap;

std::vector<ul> order_slashburn(const Adjlist &g);
void slash(const Adjlist &g, Bheap &hubs, Bheap &spokes, const ul u);

#endif
