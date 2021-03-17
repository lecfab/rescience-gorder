#include <iostream>
#include <iomanip>
#include <string>

#include <vector>
#include <cmath>
#include <algorithm>

#include "utils/CLI11.h" // options parser
#include "utils/tools.h"
#include "utils/inout.h"
#include "utils/edgelist.h"
#include "utils/adjlist.h"
#include "order/order_minla.h"


using namespace std;


int main(int argc, char** argv){
	TimeBegin()

  CLI::App app{"Test an algorithm with different parameters to tune them efficiently."};

  string filename, algo_name, output_file="parametrise.auto.txt";
  int range = 9, number = 1;
  // bool directed;
  app.add_option("file", filename, "Text file: list of `a b` edges with nodes IDs ranging from 0 to N-1")->required();
  app.add_option("algo", algo_name, "Algorithm with parameters to tune")->required()
    ->check(CLI::IsMember({"minla", "minloga","gorder","local"}, CLI::ignore_case));
  // app.add_flag("-d,!-u,--directed,!--undirected", directed,
  //     "Specify if the graph is directed or undirected; multiple edges are not accepted");
  app.add_option("-o,--output", output_file, "File in which to output the order")->capture_default_str();
  app.add_option("-r,--range", range, "Number of values to test for each parameter")->capture_default_str();
  app.add_option("-n,--number", number, "Number of repeated tests for randomised algorithms")->capture_default_str();

  CLI11_PARSE(app, argc, argv);

  Info("Reading edgelist from file " << argv[1])
  ifstream file(filename);
  Edgelist h(file);
  Info("Number of nodes: " << h.n)
  Info("Number of edges: " << h.e)
  TimeStep("Read edges")

  Info("Converting to bidirected adjacency list");
  Uadjlist g(h);
  TimeStep("Adjlist")

  unsigned seed = time(0);
  srand(seed);
  Info("Random seed (srand from cstdlib): "<< seed)

  Info("Tuning parameters")
  ofstream output(output_file);

  if(algo_name == "minla") minla_parametrise(output, g, range, number);
  else if(algo_name == "minloga") minloga_parametrise(output, g, range, number);
  else if(algo_name == "local") minla_localsearch(output, g);
  else Alert("Unknown parametrisation")
  TimeStep("Tuning")


  // Info("Output into file " << argv[3])
  // h.print_c(argv[3]);
  // TimeStep("Output")

  TimeTotal()
	return 0;
}
