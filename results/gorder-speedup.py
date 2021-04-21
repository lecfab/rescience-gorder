# Take time measurement files for various datasets, orderings and algorithms
# Create plots of runtime relative to Gorder
# Create skyscrapper histogram to show rank frequency of orderings

from gordermain import *


folder = r____()
print("Relatve runtime and ranking frequency")

# ----------------------------------------
# extract data from time-measurement files
# ----------------------------------------
data = []
missing_datasets = []
for dataset in datasets:
    dataset_seen = False
    order_data = []
    for order in orders:
        algo_data = [0] * A
        for algo in algos:
            try:
                for l in through_file("{}time-{}-{}-{}.txt".format(folder, dataset, order, algo), silent=True):
                    try: # if time and clock-cycles are available
                        algo_name, time, clock = l.split("\n")[0].split("\t")
                        time, clock = float(time), float(clock) / 1000
                        if True and time / 1.5 < clock < time * 1.5: time = clock
                    except: # if only time is available
                        algo_name, time = l.split("\n")[0].split("\t")
                        time = int(time)
                    if algo_name == algo:
                        dataset_seen = True
                        algo_data[algos.index(algo_name)] += time / 1000 # ms to s
            except IOError as e:
                pass
        order_data.append(algo_data)
    data.append(order_data)
    if not dataset_seen: missing_datasets.append(dataset)

if len(missing_datasets): print("Warning: datasets are missing: " + ",".join(missing_datasets))


# ----------------------------------------
# -- draw figure of aggregated ranking ---
# ----------------------------------------
startD = 0 # set to 1 to remove epinion
display_order = list(map(orders.index, ["gorder", "rcm", "dfs", "deg-", "slashburn", "original", "minloga", "minla", "ldg", "rand"]))
def color(s): # s = score from 0 to O
    return (int(255-s/O*155)/255, int(255-s/O*200)/255, int(225-s/O*225)/255)

# ----- compute frequency of ranks for each order -----
def rank(d, order, a):
    return len([0 for o in range(O) if data[d][o][a] < data[d][order][a]])

ranks, cumul = [], []
for o in range(O):
    freq = [0] * O
    for d in range(startD, D):
        for a in range(A):
            freq[rank(d, o, a)] += 1
    # print(orders[o], "\t", freq)
    ranks.append(freq)

for o in range(O):
    cumul.append([sum(ranks[o][:r]) for r in range(O+1)])


# ----- create skyscrapper histogram -----
plt.figure(figsize=(140/18,80/18))
# plt.grid(axis="x", alpha=.9, color="#666")
plt.xlim(0,(D-startD)*A)
plt.yticks([-o for o in range(O)], labels=[order_names[orders[o]] for o in display_order])

for r in range(O):
    plt.barh([-o for o in range(O)], [ranks[o][r] for o in display_order], left=[cumul[o][r] for o in display_order],
        height=(O-r)/O*.9,
        color=color(r), edgecolor="black")

# ----- save figure -----
save_img("{}img-ranking.pdf".format(folder))




# ----------------------------------------
# -- draw figure of runtime comparison ---
# ----------------------------------------


def generate_barplots(fig5 = True):
    fig, ax = plt.subplots(5,2, figsize=(240/25,297/25), sharex='col', sharey='row')
    fig.tight_layout(pad=0, w_pad=0, h_pad=.4)
    # plt.suptitle("Relative speedup of Gorder")
    bar_width = .085  # the width of each little bar
    tick_min, tick_max, tick_step = -.3, 1.0, .2 # y ticks around Gorder result
    tick_range = range(int(tick_min / tick_step) -1, int(tick_max / tick_step) +1)

    worse_score = 0
    for a in range(A):
        # ----- choose subplot for each algorithm -----
        algo = algos[a]
        aa = a+1 if a > 0 else a
        sub = aa // 2, aa % 2
        # print(algo, sub)

        # ----- print title and axis -----
        if fig5: ax[sub].set_title('{}'.format(algo_names.get(algo, algo.capitalize())), x=.163, y=.85, loc='center')
        else:    ax[sub].set_title('{}'.format(algo_names.get(algo, algo.capitalize())), y=.85, loc='center')

        if fig5:
            nb_bars, nb_groups = O-1, D
            xticks = np.arange(nb_groups)  # x labels locations
            ax[sub].set_xticks(xticks)
            ax[sub].set_xticklabels(datasets, rotation=45, ha="right", rotation_mode="anchor") #, y=-.1)
        else:
            nb_bars, nb_groups = D, O-1
            xticks = np.arange(nb_groups)
            ax[sub].set_xticks(xticks)
            ax[sub].set_xticklabels([order_names[orders[o]] for o in range(nb_groups)], rotation=45, ha="right", rotation_mode="anchor")
        ax[sub].set_xlim(-bar_width*(nb_bars+2)/2, (nb_groups-1)+bar_width*(nb_bars+2)/2)


        # ax[sub].set_ylabel('Relative duration to Gorder')
        ax[sub].set_yticks([tick_step*i for i in tick_range])
        ax[sub].set_yticklabels([int(tick_step*(i+1/tick_step)*10)/10 for i in tick_range])
        ax[sub].set_ylim(tick_min, tick_max)
        ax[sub].axhline(y = 0, color = 'black', linestyle = '-')
        for i in range(int(fig5),len(xticks),2):
            ax[sub].axvspan(xticks[i]-.5, xticks[i]+.5, color='#ddd', alpha=0.5, linewidth=1)

        # ----- find Gorder (reference time) for every dataset ------
        go = orders.index("gorder")
        reference = []
        for d in range(D):
            reference.append(data[d][go][a])
            if reference[d] == 0:
                reference[d] = 1
            elif fig5:
                # print(reference[d])
                ax[sub].text(xticks[d], -.27, time_format(reference[d]), horizontalalignment='center', fontsize=8)

        # ----- create histogram bars for each order -----
        if fig5:
            for o in range(O):
                if orders[o] == "gorder": continue
                for d in range(D):
                    data[d][o][a] = data[d][o][a] / reference[d] - 1
                    if data[d][o][a] < -1: data[d][o][a] = 0

                performances = [data[d][o][a] for d in range(D)]
                offset = (o - (nb_bars-1)/2)*bar_width
                ax[sub].bar(xticks + offset, performances, bar_width, label=order_names[orders[o]])
                worse_score = max(worse_score, max(performances)+1)
        else:
            for d in range(D):
                offset = (d - (nb_bars-1)/2)*bar_width
                ax[sub].bar(xticks + offset, [data[d][o][a] for o in range(O-1)], bar_width, label=f"${datasets[d]}$")


    # ----- put legend in the empty subplot [0,1] -----
    legend_lines, legend_labels = ax[0,0].get_legend_handles_labels()
    ax[0,1].axis('off')
    ax[0,1].legend(legend_lines, legend_labels,
            loc="center", ncol=3, columnspacing=1,
            frameon=False, borderaxespad=0.,
            title=("Ordering" if fig5 else "Dataset"), title_fontsize=14, fontsize=11)

    # ----- save figure -----
    if fig5:
        print("Worse score ", worse_score)
        save_img("{}img-speedup.pdf".format(folder))
    else:
        save_img("{}img-speedup-grouped.pdf".format(folder))

generate_barplots()
generate_barplots(False)
