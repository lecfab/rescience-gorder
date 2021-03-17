#ifndef INOUT_H
#define INOUT_H

#include <fstream> // ofstream: write, ifstream: read, fstream: read and write from/to files
#include "tools.h"


//reading the edgelist from file
// void c_printedgelist(const edgelist &g, const char* output);
// edgelist readedgelist(std::ifstream &file);

ul get_number_vertices(std::ifstream &file);

// edgelist c_readedgelist(char* filename);

std::vector<ul> read_order(std::ifstream &file);
// Experiment: FILE* seems to be 20x faster than ofstream <<
void c_printorder(const std::vector<ul> &rank, const ul n, const char* output);


#endif
