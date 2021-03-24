#!/bin/bash
echo "Cache sizes for each processor of this machine:"
for p in {0..1000}
do
   folder=/sys/devices/system/cpu/cpu${p}
   if ! [ -f "$folder/cache/index0/size" ]; then
       break
   fi

   printf "  Processor $p:\t\t"
   for l in {0..3}
   do
       s=$(cat $folder/cache/index${l}/size)
       t=$(cat $folder/cache/index${l}/type)
       printf "L$l ($t): $s \t"
   done
   echo
done

echo "Experiments will soon start"
sleep 4

remove=0 # should modified datasets be removed to avoid heavy disk usage? (90Gb if remove=1, 1Tb otherwise)
if [ $1 == "laptop" ]; then
    echo "Benchmarks on laptop"
    perftool="sudo ../pmu-tools/ocperf stat -e task-clock,cycles,cpu-cycles,instructions,branches,branch-misses,L1-dcache-loads,L1-dcache-load-misses,LLC-loads,LLC-load-misses,L1-icache-loads,L1-icache-load-misses,dTLB-loads,dTLB-load-misses,iTLB-loads,iTLB-load-misses,cycle_activity.cycles_l1d_miss,cycle_activity.cycles_l2_miss,cycle_activity.cycles_l3_miss,cycle_activity.cycles_mem_any,cycle_activity.stalls_l1d_miss,cycle_activity.stalls_l2_miss,cycle_activity.stalls_l3_miss,cycle_activity.stalls_mem_any,cycle_activity.stalls_total,resource_stalls.any,resource_stalls.sb"
elif [ $1 == "khiva" ]; then
    echo "Benchmarks on server Khiva"
    # perftool="../pmu-tools/ocperf stat -e task-clock,cycles,cpu-cycles,instructions,branches,branch-misses,L1-dcache-loads,L1-dcache-load-misses,LLC-loads,LLC-load-misses,L1-icache-load-misses,dTLB-loads,dTLB-load-misses,iTLB-loads,iTLB-load-misses,cycle_activity.cycles_l1d_pending,cycle_activity.cycles_l2_pending,cycle_activity.cycles_no_dispatch,cycle_activity.stalls_l1d_pending,cycle_activity.stalls_l2_pending,resource_stalls.any,resource_stalls.sb"
    perftool="../pmu-tools/ocperf stat -e task-clock,cpu-cycles,instructions,L1-dcache-loads,L1-dcache-load-misses,LLC-loads,LLC-load-misses,cycle_activity.cycles_l1d_pending,cycle_activity.cycles_l2_pending"
    remove=1
elif [ $1 == "mesu" ]; then
    echo "Benchmarks on cluster Mesu"
    perftool="../pmu-tools/ocperf stat -e task-clock,cpu-cycles,instructions,L1-dcache-loads,L1-dcache-load-misses,LLC-loads,LLC-load-misses,cycle_activity.cycles_l1d_pending,cycle_activity.cycles_l2_pending"
else
    echo "Impossible to measure cache in unknown machines. Please install ocperf and set up this script to get cache measurements."
    perftool=""
    exit
fi


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
orders=(original rand minla minloga rcm deg- dfs slashburn ldg gorder) # deg+
algos=(nq bfs dfs tarjan bellman pr ds kcore diameter)

repeat=10
dirn=$((0 + $RANDOM % 10000))
dirr=../results/r${dirn}
python_draw=../results/draw-orders-comparison.py

mkdir $dirr
echo "Results will be found in $dirr "
cat run-cache.sh > $dirr/run_backup.sh
cat $python_draw > $dirr/draw_backup.py

for dataset in ${!datasets[*]}
do
    echo "################## ${dataset_name[$dataset]} ##################"
	for order in ${orders[*]}
	do
        tee=${dirr}/tee-${dataset_name[$dataset]}-${order}
        perf=${dirr}/perf-${dataset_name[$dataset]}-${order}

        echo "    --------- ${order} ---------"
        input=../datasets/${datasets[$dataset]}.txt
        echo "    ${input}"
        echo
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

        for algo in ${algos[*]}
    	do
            results=${dirr}/time-${dataset_name[$dataset]}-${order}-${algo}.txt
            if [ -f "$results" ]; then
                echo "    Results already exist."
                cat $results
            else
                cmd_benchmark="$perftool -o $perf-$algo.txt ./benchmark $output -a $algo -o $results -l $repeat"
                echo "    $cmd_benchmark"
                $cmd_benchmark | tee $tee-benchmark-${algo}.txt
                echo
            fi
            sleep 1
            echo
        done

		#cat $file
        if [ $remove == 1 ] && [ $order != "original" ]; then
            echo "    Removing edgelist $output"
            rm $output
        fi
        echo
	done
    echo
done

# echo "Creating figures for orders comparison"
# python3 $python_draw $dirn
