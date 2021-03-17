// See header for general documentation

#include "order_minla.h"
#include "../utils/tools.h"
#include "../utils/adjlist.h"
#include "../order/order_rand.h"
#include "../algo/algo_bfs.h"

using namespace std;


// --------------------------------------------------
// -------------- Simunated annealing ---------------
// --------------------------------------------------

AnnealingOutput annealing_swap(const Adjlist &g, const ul S, const double K, double norm(double)) {
  srand((unsigned) time(0));
  vector<ul> rank = order_identity(g.n); // random_shuffle( rank.begin(), rank.end() );
  vector<pair<ul,ul>> recovery;

  double initial = energy_rank(g, rank, norm);
  double energy = initial, best = initial;
  // Debug("Initial energy: "<<initial)

  for(ul step = 0; step < S; ++step) {
    auto uv = random_pair(g.n); // defined in tools.h, so that u!=v
    ul u = uv.first, v = uv.second;

    const double T = 1.0 - ((double) step) / S;
    double e = energy_swap(g, rank, u, v, norm);
    if(e < 0 or boltzmann_chance(e, K*T)) {
      swap(rank[u], rank[v]);
      // if(step % 10000 == 0) Debug(step<<": energy "<<energy<<", e "<<e)
      energy += e;
      // Debug("Step " << step << "/"<<S<<" : energy="<< energy << " delta="<<e <<" after swapping "<<u<<" "<<v)

      // if(K==0 and e == best and ++nonimproving > g.n) break; // local search stops when not increasing
      // else
      if(energy > best)
        recovery.push_back(uv);
      else {
        best = energy;
        // Debug("Best energy is now " << best << "   removed recovery: "<<recovery.size())
        vector<pair<ul,ul>>().swap(recovery); // https://www.techiedelight.com/delete-vector-free-memory-cpp/
      }
    }
    // if(step%g.n==1) safety_check(g, rank, energy, norm);
  }

  // cancel last swaps until back to best rank
  for(size_t i=0; i<recovery.size(); ++i) {
    auto uv = recovery[recovery.size()-1-i];
    swap(rank[uv.first], rank[uv.second]);
  }

  energy = energy_rank(g, rank, norm);
  if(energy > best*1.001) // tolerance 1/1000 because of floating point computation
    Alert("Recovered energy is " << energy << " instead of "<<best)

  return AnnealingOutput({.rank=rank, .initial=initial, .best=best});
}

bool boltzmann_chance(double delta, double kT) { // delta should always be positive
  if (kT == 0) return false;
  double r = static_cast <double> (rand()) / static_cast <double> (RAND_MAX); // [0,1]
  double chance = exp(-delta / kT);
  // if(chance > r) Debug("boltzmann_chance "<<chance<<" delta "<<delta<<" kT "<<kT)
  return chance > r;
}


double energy_rank(const Adjlist &g, const vector<ul> &rank, double norm(double)) {
  double e = 0;
  for (ul u = 0; u < g.n; u++) {
    const double rank_u = rank[u];
    for(const ul &v : g.neigh_iter(u)) {
      // if(v > u) continue;
      e += norm(rank_u - ((double)rank[v]));
    }
  }
  return e;
}

double energy_swap(const Adjlist &g, const vector<ul> &rank, const ul &u, const ul &v, double norm(double)) {
  double e = 0;
  const double rank_u = rank[u], rank_v = rank[v];
  // Debug(u << " (rank "<<rank_u<<"), "<<v<<" (rank "<<rank_v<<")")
  for(const ul &w : g.neigh_iter(u)) {
    if(w == v) continue;
    e = e - norm(rank_u - ((double)rank[w])) + norm(rank_v - ((double)rank[w]));
  }
  for(const ul &w : g.neigh_iter(v)) {
    if(w == u) continue;
    e = e - norm(rank_v - ((double)rank[w])) + norm(rank_u - ((double)rank[w]));
  }
  return 2*e;
}


// --------------------------------------------------
// -------------------- Ordering --------------------
// --------------------------------------------------

vector<ul> order_minla(const Adjlist &g) {
  Info("Minla order")
  const ul S = g.e;   // result of tuning: S shoud be as big as possible
  const double K = (double) g.e / g.n; // result of tuning: K can be 1 or lower without impact
  AnnealingOutput result = annealing_swap(g, S, K, abs);
  Info("Energy decreased from "<< result.initial <<" to "<< result.best)
  return result.rank;
}

vector<ul> order_minloga(const Adjlist &g) {
  Info("Minloga order")
  const ul S = g.e;   // result of tuning: S shoud be as big as possible
  const double K = (double) g.e / g.n; // result of tuning:
  AnnealingOutput result = annealing_swap(g, S, K, log_abs);
  Info("Energy decreased from "<< result.initial <<" to "<< result.best)
  return result.rank;
}


// --------------------------------------------------
// -- Parametrisation: steps and standard energy k --
// --------------------------------------------------

void minla_parametrise(ofstream &output, const Adjlist &g, const int range, const int repetitions) {
  Info("Parametrisation for minla")
  // double E_max = log(g.e) * g.e, E_min = g.n;
  double E_max = 2 * g.e * g.n, E_min = 1.0 / E_max;
  output << "data = [\n['MinLA', ";
  annealing_parametrise(output, g, range, repetitions, E_min, E_max, abs);
}

void minloga_parametrise(ofstream &output, const Adjlist &g, const int range, const int repetitions) {
  Info("Parametrisation for minloga")
  double E_max = 2.0* g.n, E_min = 1.0 / (2* g.n);
  output << "data = [\n['MinLogA', ";
  annealing_parametrise(output, g, range, repetitions, E_min, E_max, log_abs);
}

void minla_localsearch(ofstream &output, const Adjlist &g) {
  localsearch(output, g, abs);
}
void localsearch(ofstream &output, const Adjlist &g, double norm(double)) {
  Info("Testing localsearch")

  srand((unsigned) time(0));
  vector<ul> rank = order_identity(g.n); // random_shuffle( rank.begin(), rank.end() );
  double initial = energy_rank(g, rank, norm);
  double energy = initial;
  Debug("Initial energy: "<<initial)

  output << "steps,energy" << endl;
  output << "0," << initial << endl;

  ul step = 0;
  ul hardness = 0;
  while(++step) {
    auto uv = random_pair(g.n); // defined in tools.h, so that u!=v
    ul u, v, cu=0, cv=0;
    double e = 1;
    for(cu=0; cu<g.n and e>=0; ++cu) {
      u = (uv.first + cu*37) % g.n;
      for(cv=0; cv<g.n and e>=0; ++cv) {
        v = (uv.second + cv*23) % g.n;
        e = energy_swap(g, rank, u, v, norm);
      }
    }
    hardness += cu*g.n + cv;

    if(e < 0) {
      swap(rank[u], rank[v]);
      energy += e;
      if(step % 1000 == 0) {
        Debug(step << "," << energy << "," << hardness)
        output << step << "," << energy << "," << hardness << endl;
        hardness = 0;
      }
    }
    else {
      output << step << "," << energy << "," << hardness << endl;
      Info("No more hope after "<<step<<" steps.")
      break;
    }
  }
}

void annealing_parametrise(ofstream &output, const Adjlist &g, const int range, const int repetitions, const double E_min, const double E_max, double norm(double)) {
  double S_min = g.n/4.0, S_max = g.e*log(g.e)/4.0;
  // double S_min = 2000000000, S_max = S_min;
  double S_step = (range > 1) ? (log(S_max) - log(S_min)) / (range-1) : 0;
  double E_step = (range > 1) ? (log(E_max) - log(E_min)) / (range-1) : 0;

  Info("Testing "<<range<<" values for Steps in ["<<S_min<<", "<<S_max<<"]")
  Info("Testing "<<range<<" values for    kB in ["<<E_min<<", "<<E_max<<"]")

  double best = -1;
  int recommanded_S=0;
  double recommanded_E=0;

  output << "'steps', 'kB', 'min', 'max', 'median', 'avg']";
  for (int S_count = 0; S_count<range; S_count++) {
    ul S = exp(log(S_min) + S_step * S_count);

    for (int E_count = 0; E_count<range; E_count++) {
      double E = exp(log(E_min) + E_step * E_count);
      Info("Level "<< S_count+1 << "/"<<range <<" for steps and "<<E_count+1<<"/"<<range<<" for standard energy")
      Debug(S << " "<<E)

      vector<double> results;
      double avg = 0;
      for (int i = 0; i < repetitions; i++) {
        double energy = annealing_swap(g, S, E, norm).best;
        results.push_back(energy);
        avg += energy / repetitions;
      }
      sort(results.begin(), results.end());
      double median = results[results.size()/2];
      output << ",\n[" << S << "," << E << "," << results[0] << "," << results.back()
             << "," << median << "," << avg << "]";

      if(median < best or best < 0) {
        best = median; recommanded_S = S; recommanded_E = E;
      }
    }
  }
  output << "\n]" << endl;

  Info("Recommandation: Steps="<<recommanded_S<<" and kB="<<recommanded_E)
}

// double minla_parametrise_result(const Adjlist &g, int S, double K, double norm(double)) {
//   vector<ul> rank = order_identity(g.n);
//
//   double initial = energy_rank(g, rank, norm);
//   double energy = initial, best = initial;
//   for(int step = 0; step < S; ++step) {
//     const double T = 1.0 - ((double) step) / S;
//
//     auto uv = random_pair(g.n); ul u = uv.first, v = uv.second;
//     double e = energy_swap(g, rank, u, v, norm);
//     if(e <= 0 or boltzmann_chance(e, K*T)) {
//       swap(rank[u], rank[v]);
//       energy += e;
//     }
//     best = min(best, energy);
//   }
//   return best;
// }


// --------------------------------------------------
// --- Debug: eneregy integration = recomputation ---
// --------------------------------------------------

bool safety_check(const Adjlist &g, const vector<ul> &rank, const double energy, double norm(double)) {
  Debug("safety_check")
  double compute_energy = energy_rank(g, rank, norm);
  if(compute_energy != energy)
    Alert("Energy is now "<<energy<<" instead of "<<compute_energy)
  return true;
}
