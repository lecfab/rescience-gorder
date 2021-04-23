# Take cache counters for various datasets, orderings and algorithms
# Output cache-miss and CPU time of Gorder relative to Original order

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

from gordermain import *

folder = r____()

for dataset in reversed(datasets):
    try:
        with open("{}perf-{}-gorder-{}.txt".format(folder, dataset, algos[0])) as f: break
    except: continue


print("CPU execution and cache stall of Gorder and Original on", dataset)
print()

perf = ["cpu-cycles", "cycle_activity_cycles_l2_pending"]
specs = extract_perf("{}perf-{}-gorder-{}.txt".format(folder, dataset, algos[0]), perf)
if None in specs:
    print("Error: required metrics", ", ".join(perf))
    exit()


gdata_stall, gdata_cpu = [], []
odata_stall, odata_cpu = [], []
for algo in algos:
    gperf = extract_perf("{}perf-{}-gorder-{}.txt".format(folder, dataset, algo), perf)
    operf = extract_perf("{}perf-{}-original-{}.txt".format(folder, dataset, algo), perf)
    print(algo, gperf, operf)

    gdata_stall.append(gperf[1] / operf[0])
    gdata_cpu.append((gperf[0]-gperf[1]) / operf[0])
    odata_stall.append(operf[1] / operf[0])
    odata_cpu.append((operf[0]-operf[1]) / operf[0])

def plot_bars(title, cpu, stall):
    plt.figure(figsize=(100/25,80/25))
    plt.grid(axis="y", alpha=.9, color="#666")
    plt.xticks([i for i in range(len(algos))], labels=list(algo_names.values()), rotation=45)
    plt.ylim(0,1)
    plt.ylabel("Time consumption ratio")
    plt.bar(algos, cpu, color="#bbb", edgecolor="black", label="CPU execute")
    plt.bar(algos, stall, bottom=cpu, color="black", edgecolor="black", label="Cache stall")
    plt.legend(framealpha=.9)

    save_img("{}img-cache-{}.pdf".format(folder, title))

plot_bars("gorder", gdata_cpu, gdata_stall)
plot_bars("original", odata_cpu, odata_stall)
