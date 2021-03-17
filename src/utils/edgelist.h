#ifndef EDGELIST_H
#define EDGELIST_H

#include "tools.h"

typedef std::pair<ul, ul> edge;

struct Edgelist {
public:
  ul n;                    // number of nodes
  ul e;                    // number of edges
  std::vector<edge> edges; // list of edges

  Edgelist(ul n, ul e);
  Edgelist(std::ifstream &file);
  void sort_edges();
  void apply_rank(const std::vector<ul> &rank);
  void to_dag();
  void print_c(const char *output) const;

  ul get_degIn(const ul &u);
  ul get_degOut(const ul &u);
  ul get_deg(const ul &u);

  void print_some(ul a) const;

  // protected:
  bool deg_computed = false;
  void compute_degrees(bool force = false);
  std::vector<ul> degOut;
  std::vector<ul> degIn;
  std::vector<ul> deg;
};

#endif
