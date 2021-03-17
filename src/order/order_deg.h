#ifndef ORDER_DEG_H
#define ORDER_DEG_H

#include "../utils/tools.h"

struct Keyvalue;
struct Edgelist;
struct Adjlist;

inline bool compare_nodedeg_desc (Keyvalue &a, Keyvalue &b);
inline bool compare_nodedeg_asc (Keyvalue &a, Keyvalue &b);

// ----------------- From edgelist ------------------
std::vector<ul> rank_from_deg(const std::vector<ul> &deg, const ul &n, bool desc=true);
std::vector<ul> order_deg(Edgelist &g, bool desc=true);
std::vector<ul> order_degOut(Edgelist &g, bool desc=true);
std::vector<ul> order_degIn(Edgelist &g, bool desc=true);

// ----------------- From adjlist -------------------
std::vector<ul> order_degOut(Adjlist &g);
std::vector<ul> order_degIn(Adjlist &g);

#endif
