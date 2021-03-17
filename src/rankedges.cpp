#include <iostream>
#include <iomanip>
#include <string>

#include <vector>
#include <cmath>
#include <algorithm>

#include "utils/tools.h"
#include "utils/inout.h"
#include "utils/edgelist.h"

using namespace std;


int main(int argc, char** argv){
	TimeBegin()
  if(argc <= 3) {
    cout << "./rankedges EDGES RANK OUTPUT" << endl;
    cout << "* EDGES is the initial edge list" << endl;
    cout << "* RANK is the list of new indices for each node" << endl;
    cout << "* OUTPUT is the file to output the resulting edgelist" << endl;
    cout << "returns a (sorted) edge list where nodes have new indices defined by RANK" << endl;
    return 1;
  }

  Info("Reading edgelist from file " << argv[1])
  ifstream edgefile(argv[1]);
  Edgelist h(edgefile);
  Info("Number of nodes: " << h.n)
  Info("Number of edges: " << h.e)
  TimeStep("Read edges")

  Info("Reading rank from file " << argv[2])
  ifstream rankfile(argv[2]);
  vector<ul> rank = read_order(rankfile);
  TimeStep("Read rank")

  Info("Applying rank on edges")
  h.apply_rank(rank);
  TimeStep("Apply")

  Info("Sorting edges according to new indices")
  h.sort_edges();
  TimeStep("Sort")

  Info("Output into file " << argv[3])
  h.print_c(argv[3]);
  TimeStep("Output")

  TimeTotal()
	return 0;
}
