/*
Benchmark for Gorder replication in ReScience
  Hao Wei, Jeffrey Xu Yu, Can Lu, and Xuemin Lin.
  Speedup Graph Processing by Graph Ordering.
  In Proceedings of SIGMOD '16, 2016.

Algorithms:
  *Neighbors Query (NQ)									randomly chooses 10n nodes for each dataset and accesses their out-neighbors.
  *Breadth-First Search (BFS) [20]
  *Depth-First Search (DFS) [20]
  *Strongly Connected Component (SCC) detection [48]
  *Shortest Paths (SP) by the Bellman-Ford algorithm [20]
  *PageRank (PR) [39]										stopped after 100 iterations in every dataset.
  *Dominating Set (DS) [19]
  *graph decomposition (Kcore) [6]
  *graph diameter (Diam)									longest shortest distance of 5,000 randomly chosen nodes.

Orders:
  *Original		*random would make more sense
  *MINLA			min(u,v in E)   |u-v|			simulated annealing
  *MLOGA			min(u,v in E)   log|u-v|		simulated annealing
  *RCM				reverse Cuthill-McKee algorithm [22], which is the well known solution for reducing graph bandwidth: https://www.boost.org/doc/libs/1_37_0/libs/graph/doc/cuthill_mckee_ordering.html
  *DegSort		sorting nodes in descending order of in-degree.
  *CHDFS			children-depth-first traversal which is proposed in [4].
  *SlashBurn		graph compression algorithm [29].
  *LDG				streaming graph partitioner [49].
  METIS			de-facto standard offline graph partitioner [30]
  *Gorder			w=5								https://github.com/datourat/Gorder

*/


#include <iostream>
#include <iomanip>
#include <string>

#include <vector>
#include <cmath>

#include "utils/CLI11.h" // options parser
#include "utils/tools.h"
#include "utils/inout.h"
#include "utils/edgelist.h"
#include "utils/adjlist.h"
#include "order/order_rand.h"
#include "order/order_deg.h"
#include "algo/algo_nq.h"
#include "algo/algo_bfs.h"
#include "algo/algo_dfs.h"
#include "algo/algo_bellman.h"
#include "algo/algo_diameter.h"
#include "algo/algo_kcore.h"
#include "algo/algo_tarjan.h"
#include "algo/algo_pagerank.h"
#include "algo/algo_dominatingset.h"

using namespace std;


int main(int argc, char** argv){
	TimeBegin()
  CLI::App app{"Execute various algorithms on a given directed graph."};

  string filename, algo_name="no", output_file="";
  bool algo_all=false; int attempts=10; // bool directed;
  app.add_option("file", filename, "Text file: list of `a b` edges with nodes IDs ranging from 0 to N-1")->required();
  // app.add_flag("-d,!-u,--directed,!--undirected", directed, "Specify if the graph is directed or undirected; multiple edges are not accepted");
  app.add_option("-a,--algo", algo_name, "Algorithm to execute on the graph")
    ->check(CLI::IsMember({
      "nq",
      "bfs","dfs",
      "bellman",
      "diameter",
      "tarjan",
      "kcore",
      "pr",
      "ds","dominatingset"}, CLI::ignore_case));
  app.add_flag("-A,--all", algo_all, "Execute all algorithms consecutively and save time measurements")->capture_default_str();
  app.add_option("-o,--output", output_file, "File in which to output time measurements")->capture_default_str();
  app.add_option("-l,--loops", attempts, "Number of attempts for random algorithms")->capture_default_str();

  CLI11_PARSE(app, argc, argv);
  ofstream output(output_file);

	// --------------------------------------------------
	// -- Convert edgelist file into Adjlist structure --
	// --------------------------------------------------
	Info("Reading edgelist from file " << filename)

	ifstream file(filename);
	Edgelist h(file); //*h = c_readedgelist(argv[1]);

	Info("Number of nodes: " << h.n)
	Info("Number of edges: " << h.e)

	TimeStep("Read")
  TimeRecStep("read", output)

	Info("Converting to directed adjacency list")
  Dadjlist g(h);

	TimeStep("Adjlist")
  TimeRecStep("adjlist", output)


	if(algo_all or algo_name != "no") {
    srand (time(NULL));
		// --------------------------------------------------
		// ----- Execute an algorithm over the Adjlist ------
		// --------------------------------------------------

    TimeTotal()
    TimeRecTotal("prepare", output)

		if(algo_all or algo_name == "nq") {
      algo_nq(g, g.n);
      TimeStep("NQ")
      TimeRecStep("nq", output)
    }
		if(algo_all or algo_name == "bfs") {
      for (int i = 0; i < attempts; i++) {
        algo_bfs(g, rand() % g.n);
        TimeStep("BFS")
        TimeRecStep("bfs", output)
      }
    }
		if(algo_all or algo_name == "dfs") {
      for (int i = 0; i < attempts; i++) {
        algo_dfs(g, rand() % g.n);
        TimeStep("DFS")
        TimeRecStep("dfs", output)
      }
    }
    if(algo_all or algo_name == "tarjan") {
      for (int i = 0; i < attempts; i++) {
        algo_tarjan(g, rand() % g.n);
        TimeStep("SCC")
        TimeRecStep("tarjan", output)
      }
    }
    if(algo_all or algo_name == "bellman") {
      for (int i = 0; i < attempts; i++) {
        algo_bellman_complete(g, rand() % g.n);
        TimeStep("SP")
        TimeRecStep("bellman", output)
      }
    }
    if(algo_all or algo_name == "pr") { // warning: uses Edgelist and not Adjlist
      algo_pagerank(h, attempts * attempts);
      TimeStep("PR")
      TimeRecStep("pr", output)
    }
    if(algo_all or algo_name == "ds" or algo_name == "dominatingset") {
      for (int i = 0; i < attempts; i++) {
        algo_dominatingset(g);
        TimeStep("DS")
        TimeRecStep("dominatingset", output)
      }
    }
    if(algo_all or algo_name == "kcore") {
      for (int i = 0; i < attempts; i++) {
        algo_kcore(g);
        TimeStep("Kcore")
        TimeRecStep("kcore", output)
      }
    }
    if(algo_all or algo_name == "diameter") {
      algo_diameter(g, attempts * attempts);
      TimeStep("Diam")
      TimeRecStep("diameter", output)
    }
    // -----------------------------------------
	}

	TimeTotal()
  TimeRecTotal("total", output);
	return 0;
}
