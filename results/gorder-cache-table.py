# Take cache counters for various datasets, orderings and algorithms
# Output Latex table of cache-miss rates

# ----- Example of input -----
#     3407283.482795      task-clock (msec)         #    1.000 CPUs utilized
#  9,540,291,508,200      cpu-cycles                #    2.800 GHz
#  4,038,838,258,530      instructions              #    0.42  insns per cycle
#  1,423,315,977,723      L1-dcache-loads           #  417.727 M/sec
#    275,973,897,937      L1-dcache-load-misses     #   19.39% of all L1-dcache hits
#    234,513,818,234      LLC-loads                 #   68.827 M/sec
#    119,939,474,750      LLC-load-misses           #   51.14% of all LL-cache hits
#  7,910,306,299,183      cycle_activity_cycles_l1d_pending # 2321.587 M/sec
#  7,919,923,112,081      cycle_activity_cycles_l2_pending # 2324.410 M/sec

# ----- Example of output -----
# Original & 27,7 & 16,74 \% & 3,2 & 11,54 \% & 3,81 \% \\
# MINLA & 27,8 & 17,06 \% & 3,7 & 13,28 \% & 4,11 \% \\
# MLOGA & 27,6 & 16,58 \% & 3,5 & 12,67 \% & 4,31 \% \\
# RCM & 29,1 & 12,26 \% & 2,4 & 8,41 \% & 3,26 \% \\
# DegSort & 27,0 & 15,40 \% & 3,0 & 11,18 \% & 3,48 \% \\
# CHDFS & 27,6 & 13,49 \% & 2,6 & 9,32 \% &  2,98 \% \\
# SlashBurn & 27,2 & 15,42 \% & 3,1 & 11,57 \% & 3,71 \% \\
# LDG & 28,7 & 20,39 \% & 4,0 & 13,82 \% & 4,36 \% \\
# Gorder & 27,3 & 10,8 \% & 2,0 & 7,46 \% & 3,44 \%

from gordermain import *

folder = r____()

algo = "pr"
print("Cache-miss rates for", algo_names[algo], "on all datasets")

perf = ["L1-dcache-loads", "L1-dcache-load-misses", "LLC-loads", "LLC-load-misses"]
specs = extract_perf("{}perf-{}-{}-{}.txt".format(folder, datasets[0], orders[0], algo), perf)
missing_perf = [perf[i] for i in range(len(perf)) if specs[i] is None]
warning = "Warning: tables are incomplete because of missing metrics " + ", ".join(missing_perf)
if len(missing_perf): print(warning)

def pc(x):
    return str(int(x*1000) / 10) + " \\%"
def billions(x, digits=0):
    if digits == 0: return str(int(x/ 1e9))
    return str(int(x/ 1e9 * 10**digits) / 10**digits)

def print_table(dataset):
    data = []
    for order in orders:
        specs = extract_perf("{}perf-{}-{}-{}.txt".format(folder, dataset, order, algo), perf)

        L1ref = "?" if specs[0] is None else billions(specs[0])
        L1mr = "?" if (specs[0] is None or specs[1] is None) else pc(specs[1] / specs[0])
        L3ref = "?" if specs[2] is None else billions(specs[2])
        L3r = "?" if specs[0] is None or specs[2] is None else pc(specs[2] / specs[0])
        Lmr = "?" if specs[0] is None or specs[3] is None else pc(specs[2] / specs[3])

        cols = [order_names[order], # order name
                L1ref,              # number of L1 references
                L1mr,               # L1 miss-rate
                L3ref,              # number of L3 references
                L3r,                # L1+L2 miss-rate
                Lmr                 # all-levels miss-rate
            ]
        data.append(cols)

    print("\nOn dataset", dataset)
    print("Order & L1-ref (10$^9$) & L1-mr & L3-ref (10$^9$) & L3-r & Cache-mr \\\\ \hline")
    for c in data:
        print(" & ".join(c), "\\\\")


for dataset in datasets:
    try:
        with open("{}perf-{}-gorder-{}.txt".format(folder, dataset, algo)) as f: pass
        print_table(dataset)
    except: continue

if len(missing_perf): print("\n", warning)
