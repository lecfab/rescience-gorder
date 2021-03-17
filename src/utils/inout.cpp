#include "inout.h"
#include "tools.h"
#include "edgelist.h"
#include <iostream>
#include <fstream> // ofstream: write, ifstream: read, fstream: read and write from/to files

using namespace std;


ul get_number_vertices(ifstream &file) {
  ul n=0, u, v;
  while(file >> u >> v) {
    //Debug(n << ' '<<u <<' '<< v)
    n = max3(n, u, v);
  }
  return n+1;
}


vector<ul> read_order(ifstream &file) {
  vector<ul> rank; rank.reserve(NNODES);
	for(ul i=0; file >> i;) rank.push_back(i);
	return rank;
}

// Experiment: FILE* seems to be 20x faster than ofstream <<
void c_printorder(const vector<ul> &rank, const ul n, const char* output){
  FILE *file=fopen(output,"w");
  for (ul u=0; u < n; ++u) {
    if(rank[u] >= n) {
      Alert("rank " << u << " is " << rank[u])
      continue;
    }
    fprintf(file, "%lu\n", rank[u]);
  }
  fclose(file);
}


//reading the edgelist from file
// edgelist readedgelist(ifstream &file) {
//   edgelist g;
// 	g.edges.reserve(NLINKS);
// 	g.n=0;
// 	ul u, v;
// 	while(file >> u >> v) {
// 		g.edges.push_back(edge(u, v));
// 		g.n = max3(g.n, u, v);
// 	}
// 	g.e = g.edges.size();
// 	g.n++;
// 	return g;
// }

// void c_printedgelist(const edgelist &g, const char* output) {
//   FILE *file=fopen(output,"w");
// 	for (ul i=0; i < g.e; ++i) { fprintf(file, "%lu %lu\n", g.edges[i].first, g.edges[i].second); }
// 	fclose(file);
// }

// edgelist c_readedgelist(char* filename) {
// 	FILE *file;
// 	file=fopen(filename,"r");

// 	edgelist g;
// 	g.edges.reserve(NLINKS);
// 	g.n=0;
// 	ul u, v;
// 	while (fscanf(file,"%lu %lu\n", &u, &v)==2) {
// 		g.edges.push_back(edge(u, v));
// 		g.n = max3(g.n, u, v);
// 	}
// 	fclose(file);

// 	g.e = g.edges.size();
// 	g.n++;
// 	return g;
// }


// void printorder(const vector<ul> &rank, const ul n, ofstream &file, const Adjlist &g){
// 	for (ul u = 0; u < n; ++u)
// 		file << rank[u] << " " << g.get_degree(u) << endl;
// }
