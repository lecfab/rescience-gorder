#include "tools.h"

using namespace std;

void print_time_clock(chrono::steady_clock::time_point begin, chrono::steady_clock::time_point end, clock_t cl_begin, clock_t cl_end) {
	long int diff = chrono::duration_cast<chrono::milliseconds>(end - begin).count();  int factor = 1000;
	long int diff_copy = diff;
	long int h = diff / 3600 / factor;	diff -= h * 3600 * factor;
	long int m = diff / 60 / factor;	diff -= m * 60 * factor;
	long int s = diff / factor;			diff -= s * factor;
  long int cl_hits = cl_end - cl_begin;
  long int cl_ms = (1000.* (float) cl_hits / CLOCKS_PER_SEC);
	printf("\t- Time = %ldh %ldm %lds %ld\t\tms=%ld\tclock:%ld/%ld = %ldms\n", h, m, s, diff, diff_copy, cl_hits, (long int) CLOCKS_PER_SEC, cl_ms);
}

void print_time(chrono::steady_clock::time_point begin, chrono::steady_clock::time_point end) {
	long int diff = chrono::duration_cast<chrono::milliseconds>(end - begin).count();  int factor = 1000;
	long int diff_copy = diff;
	long int h = diff / 3600 / factor;	diff -= h * 3600 * factor;
	long int m = diff / 60 / factor;	diff -= m * 60 * factor;
	long int s = diff / factor;			diff -= s * factor;
	printf("\t- Time = %ldh %ldm %lds %ld\t\tms=%ld\n", h, m, s, diff, diff_copy );
}

vector<ul> order_identity(const ul &n) {
  vector<ul> rank; rank.reserve(n);
  for (ul u = 0; u < n; ++u)
  	rank.push_back(u);
  return rank;
}
vector<ul> rank_from_order(const vector<ul> &order, const ul &n) {
  vector<ul> rank; rank.reserve(n);
  Debug("rank_from_order " << n)
  for (ul u = 0; u < n; ++u) {
    if(order[u] >= n) {
      Alert("order " << u << " is " << order[u])
      exit(1);
    }
  	rank[ order[u] ] = u;
  }
  Debug("obtained")
  return rank;
}
vector<ul> rank_from_order(const vector<ul> &order) {
  return rank_from_order(order, order.size());
}

pair<ul,ul> random_pair(const ul &n) {
  ul u = rand() % n, v = rand() % (n - 1);
  return make_pair(u, v + (v >= u));
}

// https://www.cprogramming.com/c++11/c++11-auto-decltype-return-value-after-function.html
// template <typename Builder>
// auto makeAndProcessObject (const Builder& builder) -> decltype( builder.makeObject() )
// {
//     auto val = builder.makeObject();
//     // do stuff with val
//     return val;
// }

// template <typename T>
// auto Timer (T &function, int argc, char** argv) -> decltype(function(argc, argv))
// {
// 	chrono::steady_clock::time_point begin = chrono::steady_clock::now();

// 	//time_t t1=time(NULL);
// 	auto output = function(argc, argv);

// 	chrono::steady_clock::time_point end = chrono::steady_clock::now();
// 	print_time(begin, end);
// 	return output;
// }
