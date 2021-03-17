#ifndef ORDER_MINLA_H
#define ORDER_MINLA_H

// Simulated annealing to optimise either:
// * Minimum Linear Arrangement: sum for edges (u,v) of |rank(u)-rank(v)|
// * Minimum Logarithmic Arrangement: sum for edges (u,v) of log|rank(u)-rank(v)|

// Method:
// * try a random swap of two nodes' id
// * compute the change in energy D
// * if D is negative, confirm the swap
// * if D is positive, confirm with probability exp(-D / (k*T))
// * stop after a given number of steps

// Parameters:
// * number of steps S can be set to limit time complexity. Suggested: m
// * temperature T decreases along the procedure: T = 1 - s/S   at step s
// * standard energy K of our system corresponds to the order of magnitude
//      of a tolerable loss in energy. Suggested: around E/n

#include <iostream>
#include <iomanip>

#include "../utils/tools.h"

struct Adjlist;
struct Uadjlist;

struct AnnealingOutput {
  std::vector<ul> rank;
  double initial;
  double best;
};

// Simulated annealing procedure with given steps, energy and norm tp optimise
AnnealingOutput annealing_swap(const Adjlist &g, const ul S, const double K, double norm(double));
// Returns true with probability exp(-delta / (kT))
bool boltzmann_chance(double delta, double kT);
// Computes the energy of an ranking, using a given norm function (abs or log)
double energy_rank(const Adjlist &g, const std::vector<ul> &rank, double norm(double));
// Computes the energy change triggered by swapping u and v
double energy_swap(const Adjlist &g, const std::vector<ul> &rank, const ul &u, const ul &v, double norm(double));

// Main function to optimise Minimum Linear Arrangement
std::vector<ul> order_minla(const Adjlist &g);
// Main function to optimise Minimum Logarithmic Arrangement
std::vector<ul> order_minloga(const Adjlist &g);
inline double log_abs(double x) { return log2(abs(x)); }


// Tuning of parameters
void minla_parametrise(std::ofstream &output, const Adjlist &g, const int range, const int repetitions);
void minloga_parametrise(std::ofstream &output, const Adjlist &g, const int range, const int repetitions);
void annealing_parametrise(std::ofstream &output, const Adjlist &g, const int range, const int repetitions,
              const double E_min, const double E_max, double norm(double));

void minla_localsearch(std::ofstream &output, const Adjlist &g);
void localsearch(std::ofstream &output, const Adjlist &g, double norm(double));


// Checks that energy integration is valid (not needed in prod)
bool safety_check(const Adjlist &g, const std::vector<ul> &rank, const double energy, double norm(double));


#endif
