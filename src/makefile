CC=g++ -std=c++14 #-fconcepts
# CFLAGS=-O9 -lm

DEBUG?=0 # debug option

EXEC=benchmark ord rankedges parametrise
MF=makefile # recompile when Makefile has been modified

ifeq ($(DEBUG), 1)
	CFLAGS=-Og -Wextra -g3 -D DEBUG
	o=debug.o
else
	CFLAGS=-Ofast #-Wall # https://cpluspluspedia.com/fr/tutorial/4708/compiler-et-construire
	o=o
endif


UTILS_H =	utils/tools.h \
			utils/inout.h \
			utils/edgelist.h \
			utils/adjlist.h \
			utils/heap.h \
			utils/unitheap.h
			# utils/CLI11.h
ALGOS_H = 	algo/algo_nq.h \
			algo/algo_bfs.h \
			algo/algo_dfs.h \
			algo/algo_bellman.h \
			algo/algo_diameter.h \
			algo/algo_kcore.h \
			algo/algo_tarjan.h \
			algo/algo_pagerank.h \
			algo/algo_dominatingset.h
ORDERS_H = 	order/order_deg.h \
			order/order_rand.h \
			order/order_rcm.h \
			order/order_gorder.h \
			order/order_ldg.h \
			order/order_minla.h \
			order/order_slashburn.h

UTILS_O =	$(UTILS_H:.h=.$(o))
ALGOS_O = 	$(ALGOS_H:.h=.$(o))
ORDERS_O = 	$(ORDERS_H:.h=.$(o))

all: $(EXEC)

benchmark: benchmark.$(o) $(UTILS_O) $(ALGOS_O) $(ORDERS_O)
	$(CC) $^ $(CFLAGS) -o $@

ord: ord.$(o) $(UTILS_O) $(ALGOS_O) $(ORDERS_O)
	$(CC) $^ $(CFLAGS) -o $@

rankedges: rankedges.$(o) $(UTILS_O)
	$(CC) $^ $(CFLAGS) -o $@

parametrise: parametrise.$(o) $(UTILS_O) $(ALGOS_O) $(ORDERS_O)
	$(CC) $^ $(CFLAGS) -o $@

benchmark.$(o): $(UTILS_H) $(ALGOS_H) $(ORDERS_H)
ord.$(o): $(UTILS_H) $(ALGOS_H) $(ORDERS_H)
rankedges.$(o): $(UTILS_H)
parametrise.$(o): $(UTILS_H) $(ALGOS_H) $(ORDERS_H)


algo/algo_%.$(o): $(UTILS_H)
order/order_%.$(o): $(UTILS_H)
algo/algo_diameter.$(o): algo/algo_bellman.h
algo/algo_tarjan.$(o): algo/algo_dfs.h
algo/algo_minuncut.$(o): order/order_deg.h
order/order_rcm.$(o): order/order_deg.h algo/algo_bfs.h
order/order_gorder.$(o): order/order_rcm.h
order/order_ldg.$(o): algo/algo_bfs.h

%.debug.o: %.cpp $(MF)
	$(CC) -c $< $(CFLAGS) -o $@

%.o: %.cpp $(MF)
	@$(CC) -c $< $(CFLAGS) -o $@

.PHONY: clean mrproper

clean:
	rm -rf */*.$(o) *.$(o)

mrproper: clean
	rm -rf $(EXEC)
