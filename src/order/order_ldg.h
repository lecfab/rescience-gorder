#ifndef ORDER_LDG_H
#define ORDER_LDG_H

// Linear Deterministic Greedy partitioning
//
// I. Stanton and G. Kliot.
// Streaming graph partitioning for large distributed graphs.
// In Proc. of KDD’12, 2012.

#include "../utils/tools.h"

struct Adjlist;

struct Chunk {
  ul size = 0;
  int candidate_id = -1;
  std::vector<ul> nodes;
};

struct Candidate {
  int score;
  ul chunk_id;
};


std::vector<ul> order_ldg(Adjlist &g, ul chunk_size=64);
std::vector<ul> order_ldg_pure(Adjlist &g, ul chunk_size=64);

#endif
