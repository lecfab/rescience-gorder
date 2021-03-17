#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>

#include "order_ldg.h"
#include "../utils/tools.h"
#include "../utils/adjlist.h"
#include "../algo/algo_bfs.h"

using namespace std;


vector<ul> order_ldg(Adjlist &g, ul chunk_size) {
  Info("Applying BFS order");
  vector<ul> rank_bfs = rank_from_order(algo_bfs(g));
  Info("Reordering adjacency list");
  g.reorder(rank_bfs); // check that it is using rank and not order
  // g.print_some(100);

  Info("Applying LDG");
  vector<ul> rank_ldg = order_ldg_pure(g, chunk_size);

  vector<ul> rank; rank.reserve(g.n);
  for (ul u = 0; u < g.n; u++) {
    rank[u] = rank_ldg[rank_bfs[u]];
  }
  return rank;
}

vector<ul> order_ldg_pure(Adjlist &g, ul chunk_size) {
  srand((unsigned) time(0));

  vector<Chunk> chunks(ceil((float) g.n / chunk_size));
  vector<ul> partition; partition.reserve(g.n);
  Debug("Created chunks: "<< chunks.size())

  for(ul u=0; u<g.n; ++u) {
    // Debug("Node "<<u)
    vector<Candidate> candidates;
    for (size_t i = 0; i < 5; ++i)// add random chunks in case neighbouring ones are full
      candidates.push_back(Candidate({ .score=0, .chunk_id = (ul) rand()%chunks.size() }));

    for(auto &v : g.neigh_iter(u)) {
      // Debug("\tneighbour "<< v)
      if(v >= u) break; // break if neighbours are sorted, continue otherwise
      Chunk &chunk = chunks[partition[v]]; // v is < u so it has already a chunk
      if(chunk.candidate_id == -1) {
        // Debug("\t\tis new")
        chunk.candidate_id = candidates.size();
        candidates.push_back(Candidate({ .score=1, .chunk_id=partition[v] }));
      }
      else
        candidates[chunk.candidate_id].score ++;
    }

    float max_score = 0;
    ul best_chunk = 0;
    for(Candidate &candidate : candidates) {
      int score = (candidate.score + 1) * (chunk_size - chunks[candidate.chunk_id].nodes.size()); // linear penalty
      if(score > max_score) {
        max_score = score;
        best_chunk = candidate.chunk_id;
      }
      chunks[candidate.chunk_id].candidate_id = -1; // reset for next node
    }

    partition[u] = best_chunk;
    // Debug("\t"<<u<<" added to chunk "<<best_chunk)
    if(chunks[best_chunk].nodes.size() == 0) chunks[best_chunk].nodes.reserve(chunk_size * 1.1);
    chunks[best_chunk].nodes.push_back(u);
  }

  vector<ul> rank; rank.reserve(g.n);
  ul chunk_id = 0;
  for(Chunk &chunk : chunks) {
    // Debug("Pushing chunk "<<chunk_id)
    for(ul &u : chunk.nodes) {
      rank.push_back(u);
      // Debug("\tcontains "<<u)
    }
    chunk_id++;
  }

  if(rank.size() != g.n) Alert("Rank size is wrong: " << rank.size() << " instead of " << g.n)
  return rank;
}
