#!/bin/bash
# runtime and cache measurements for each algorithm under each ordering on each dataset

# ----------------------------------------
# ---- display cache size information ----
# ----------------------------------------
echo "Cache sizes for each processor of this machine:"
for p in {0..1000000}
do
   folder=/sys/devices/system/cpu/cpu${p}
   if ! [ -f "$folder/cache/index0/size" ]; then break; fi

   printf "  Processor $p:\t"
   for l in {0..3}; do
       s=$(cat $folder/cache/index${l}/size)
       t=$(cat $folder/cache/index${l}/type)
       printf "L$l-$t: $s \t"
   done
   echo
done

# ----------------------------------------
# -- perftools options on this machine ---
# ----------------------------------------
remove=0 # should modified datasets be removed to avoid heavy disk usage? (90Gb if remove=1, 1Tb otherwise)
perftool=""
if ! command -v perf &> /dev/null; then
    echo "Warning: linux-perf is not available. To get cache measurements, see README > Tools and versions to install it."
    echo "Experiments will soon start... (runtime will be measured but not cache-miss)"
    sleep 2
elif [ -z "${1}" ]; then
    echo "Missing MODE parameter. Use pre-defined cache metrics with 'basic' or 'advanced', or edit this script to add different metrics."
    # replace the following list by a cache measurement method
    # example:
    #   perftool="../pmu-tools/ocperf stat -e cpu-cycles,L1-dcache-loads,L1-dcache-load-misses,LLC-loads,LLC-load-misses,cycle_activity.cycles_l1d_pending,cycle_activity.cycles_l2_pending"
    # warning: if the above performance counters are not available on your machine you can
    #   use different ones, but you will have to replace them in the visualisation tools.
    #   The list of available counters can be obtained with command $ ../pmu-tools/ocperf list
    # If cache-miss rates are not crucial to you, only runtime will be measured.
    #   In that case, gorder-cache-bars.py and gorder-cache-table.py will not work.
    echo "Experiments will soon start... (runtime will be measured but not cache-miss)"
    sleep 2
elif [ $1 == "basic" ]; then
    echo "Basic mode: cache metrics without cycle_activity."
    perftool="../pmu-tools/ocperf stat -e task-clock,cpu-cycles,instructions,L1-dcache-loads,L1-dcache-load-misses,LLC-loads,LLC-load-misses"
elif [ $1 == "advanced" ]; then
    echo "Advanced mode: cache and cycle_activity metrics."
    perftool="../pmu-tools/ocperf stat -e task-clock,cpu-cycles,instructions,L1-dcache-loads,L1-dcache-load-misses,LLC-loads,LLC-load-misses,cycle_activity.cycles_l1d_pending,cycle_activity.cycles_l2_pending"
fi

sleep 4

# ----------------------------------------
# ----- parameters, options, backups -----
# ----------------------------------------
datasets=(
    edgelist-epinion-75k-508k
    edgelist-pokec-1M-30M
    edgelist-flickr-2M-33M
    edgelist-livejournal-4M-68M
    edgelist-wiki-13M-437M
    edgelist-gplus-28M-462M
    edgelist-pldarc-42M-623M
    edgelist-twitter-61M-1G
    edgelist-sdarc-94M-1G
)
dataset_name=(
    epinion
    pokec
    flickr
    livejournal
    wiki
    gplus
    pldarc
    twitter
    sdarc
)
orders=(original rand minla minloga rcm deg- dfs slashburn ldg gorder)
algos=(nq bfs dfs tarjan bellman pr dominatingset kcore diameter)

repeat=10
dirn=$((0 + $RANDOM % 10000))
dirr=../results/r${dirn}
python_draw=../results/gorder-speedup.py
python_bars=../results/gorder-cache-bars.py
python_table=../results/gorder-cache-table.py

mkdir $dirr
echo "Results will be found in $dirr "
cat run-benchmark.sh > $dirr/backup-run.sh
cat $python_draw > $dirr/backup-draw.py
cat $python_bars > $dirr/backup-bars.py
cat $python_table > $dirr/backup-table.py


# ----------------------------------------
# ------- main loop of experiments -------
# ----------------------------------------

# ----------- for each dataset -----------
for dataset in ${!datasets[*]}
do
    input=../datasets/${datasets[$dataset]}.txt
    if ! [ -f $input ]; then
        echo "Warning: dataset ${dataset_name[$dataset]} does not exist."
        continue
    fi

    echo "################## ${dataset_name[$dataset]} ##################"


    # ------------ for each order ------------
    for order in ${orders[*]}; do
        tee=${dirr}/tee-${dataset_name[$dataset]}-${order}
        perf=${dirr}/perf-${dataset_name[$dataset]}-${order}

        echo "    --------- ${order} ---------"
        echo "    ${input}"
        echo
        # ------- compute and apply new ordering -------
        if [ $order == "original" ]; then
            output=../datasets/${datasets[$dataset]}.txt
        else
            output=../datasets/${datasets[$dataset]}-${order}.txt

    		if [ -f "$output" ]; then
    			echo "    Edgelist already exists."
    		else
                rank=ord.tmp-${order}.txt
                cmd_ord="./ord $input $order -d -o $rank"
                echo "    $cmd_ord"
                $cmd_ord | tee $tee-ord.txt
                echo
                cmd_rank="./rankedges $input $rank $output"
                echo "    $cmd_rank"
                $cmd_rank | tee $tee-rankedges.txt

                rm $rank
    		fi
        fi
        echo

        # ----- for each algorithm, measure runtime -----
        for algo in ${algos[*]}
    	do
            results=${dirr}/time-${dataset_name[$dataset]}-${order}-${algo}.txt
            if [ -f "$results" ]; then
                echo "    Results already exist."
                cat $results
            else
                cmd_benchmark="./benchmark $output -a $algo -o $results -l $repeat"
                if [ ! -z "${perftool}" ]; then
                    cmd_benchmark="$perftool -o $perf-$algo.txt $cmd_benchmark"
                fi
                echo "    $cmd_benchmark"
                $cmd_benchmark | tee $tee-benchmark-${algo}.txt
                echo
            fi
            sleep 1
            echo
        done

        # ------ remove edgelist if asked ------
        if [ $remove == 1 ] && [ $order != "original" ]; then
            echo "    Removing edgelist $output"
            rm $output
        fi
        echo
	done
    echo
done


# ----------------------------------------
# -------- call to plot functions --------
# ----------------------------------------
echo
echo "Creating figures for orders comparison"
python3 $python_draw $dirn
if [ ! -z "${perftool}" ]; then
    python3 $python_bars $dirn
    python3 $python_table $dirn
else
    echo "No cache measurement has been made."
fi
