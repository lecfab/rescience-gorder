# Replication of Gorder paper

This is the source code of the 2021 replication for ReScience of the paper _Speedup Graph Processing by Graph Ordering_ by Hao Wei, Jeffrey Xu Yu, Can Lu, and Xuemin Lin, published in Proceedings of SIGMOD 2016.


## Quick guide
`$ git clone https://github.com/lecfab/rescience-gorder.git` to clone the repository locally.

`$ git clone https://github.com/lecfab/pmu-tools.git` to download a wrapper for linux perftools.

`$ cd src` to enter the folder with executables.

`$ make` to compile the C++ code. Use `$ make mrproper` to remove compiled files to obtain a cleaner directory.

`$ ./run-window.sh` to tune window size in Gorder.

`$ ./run-annealing.sh` to tune simulated annealing for MinLA and MinLogA.

`$ ./run-cache.sh` to measure the runtime of all orders and algorithms on all datasets.

## Tools and versions
The C++ code uses 2014 standard library and is compiled with GCC 9.3. Flags are managed by [CLI11 1.9.1](https://github.com/CLIUtils/CLI11) (included in the code). The makefile has been tested with GNU Make 4.2.1. Visualisation tools use Python 3.8.5 and plotly.js v1.58.4. The system used for development is Linux 5.4 with Ubuntu 20.04.1.


## Repository structure
### Datasets in `datasets/`
From various sources described in our replication paper, they all follow the same format: a graph representation for nodes [0 to N-1] in a text file where each line corresponds to a directed edge of the form `a b` (i.e. a SPACE b, with a and b long unsigned integers). They can be downloaded with the provided URLs, but their format may not be exactly compatible with the present tools (presence of headers or comments, non-consecutive numbering...).

To obtain the exact format, we used `datasets/normalise-dataset.py NAME` where `NAME` is one of the datasets of the paper (epinion, pokec, flickr, livejournal, wiki, gplus, pldarc, twitter, sdarc). The initial raw files have to be stored in a `dl/` subfolder (open the Python file to see or change the exact file names). It outputs a text file that can be used in all the experiments.

### Sources in `src/`
#### Algorithms
The folder `algo` contains all the algorithms that were replicated. They serve as benchmarks to test the efficiency of different orders.

#### Orders
The folder `order` contains all the orders compared in our replication. The contribution of the initial paper is Gorder, and our code here is majorly inspired from the code the authors provide on [this link](http://github.com).

#### Data structures
The folder `utils` contains different data structures that we needed for this replication: `adjlist` and `edgelist` are two types of graph representation; `heap` and `unitheap` are data structures used for Kcore and Gorder respectively; `tools` and `inout` are general methods such as time measurement, file input and output...

### Results in `results/`
Different automated tests will create specific sub-folders in `results/` to store raw results in text files. The visualisation tools stored in the folder will use these files to create images and store them in the same sub-folders in pdf format.


## Executables programs

### Create an order
`$ ./ord DATASET ORDER -d -o RANK`

-   `DATASET` is a text file following the format described above
-   `ORDER` is the name of an order such as `deg-`, `core`, `gorder`, `slashburn`...
-   `RANK` is the name of the output text file that will contain a list of numbers from 0 to N-1 reorganised according to `ORDER` (much smaller size than `DATASET`)

### Apply the order
`$ ./rankedges DATASET RANK OUTPUT`

-   `DATASET` is a dataset following the format described above
-   `RANK` is an output of `./ord`
-   `OUTPUT` is the name of the output text file that will contain the new graph with reordered nodes (size proportional to `DATASET`)

### Test the performance of orders
`$ ./benchmark DATASET -a ALGO -l REPEAT -o RESULTS`

-   `DATASET` is a dataset following the format described above (possibly reordered using `./ord` and `./rankedges`)
-   `REPEAT` is the number of repetitions for each algorithm (the initial paper and our experiments use 10)
-   `RESULTS` is the text file in which time measurements will be written
-   `ALGO` is the name of an algorithm such as `bfs`, `tarjan`, `diameter`... Use `-A` for all of them.


## Automated tests
All tests must be run from the `src/` folder.

### Gorder window size tuning
`$ ./run-window.sh [REPEAT]` tests different window sizes for Gorder on Flickr dataset (2 million nodes), with `REPEAT` repetitions for stability of measurements (we used 100 in our experiments).

### Simulated annealing
`$ ./run-annealing.sh` tests parameters of simulated annealing (standard energy k and number of steps S). Their score is measured for MinLA and MinLogA optimisation functions, and displayed in a HTML 3D plot (to be displayed with a web browser).

### Runtime and cache performance
`$ ./run-cache.sh MACHINE` runs time measurements of 9 algorithms on 9 datasets for 10 orders. Results are stored in files `results/r????/time-DATASET-ORDER-ALGO.txt` where `????` is a random directory name.

`MACHINE` allows you to define a configuration for performance measurement tools. We use perf-tools (wrapped in pmu-tools ocperf), but different hardware events are available depending on the system. Type `$ ../pmu-tools/ocperf list` to see what your machine offers. An other measurement library can be plugged instead. For visualisation, the provided tools use specific performance counters, listed below.


## Plots and result visualisation

### Gorder window size
`gorder-window.py` creates a 2D plot to represent the efficiency of different window sizes for Gorder. For stability, the runtime measurement has to be repeated several times, and this tool plots the 90% confidence interval as well as the median for each value of w.

### Simulated annealing
`gorder-annealing.html` is a local web page that can be opened in any modern browser. It displays a 3D plot with the resulting energy obtained for each choice of parameters S (number of steps) and k (standard energy).

### Orders comparison diagrams
`gorder-speedup.py ????` analyses the runtime files in folder `r????` and create the histograms for all choices of algorithm, ordering and datasets. It also combines the result in a skyscraper histogram, where each ordering is represented with its ranking against other orderings.

### Cache-miss information
`gorder-cache-table.py ????` takes the cache-performance files in folder `r????` and prints a table (in Latex format) with cache-miss rates for each ordering, on a given dataset and algorithm. It requires the following perf-tools counters: _cpu-cycles_, _L1-dcache-loads_, _L1-dcache-load-misses_, _LLC-loads_, _LLC-load-misses_.

`gorder-cache-bars.py ????` takes the cache-performance files in folder `r????` and plots the rate of CPU execution and cache stall for Gorder and Original for each algorithm on a given dataset. It requires the following perf-tools counters: _cpu-cycles_, _cycle_activity ⋅ cycles_l1d_pending_, _cycle_activity ⋅ cycles_l2_pending_.
