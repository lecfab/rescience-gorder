#!/bin/bash
# comparison of runtime for different sizes of window in Gorder

algo=pr

dataset=edgelist-epinion-75k-508k # note that epinion is too small to obtain relevant resluts here
dataset_name=epinion
max_window=75000

# dataset=edgelist-flickr-2M-33M
# dataset_name=flickr
# max_window=2000000

declare -i loops=10
if (( $# > 0 )); then loops=$1; fi
declare -i loop=0

dirr=../results/gorder-window
python_draw=../results/gorder-window.py

mkdir $dirr
echo "Results will be found in $dirr "
cat run-window.sh > $dirr/backup-run.sh
cat $python_draw > $dirr/backup-draw.py


for loop in $(seq 0 $(($loops - 1))); do
    echo "----------------------"
    echo "--------- loop=${loop}"
    echo "----------------------"

    window=1
    while (( $window < $max_window )); do
        echo "    --------- Window=${window} ---------"
        input=../datasets/${dataset}.txt
        rank=ord.auto.txt
        output=../datasets/${dataset}-gorder${window}.txt

        if [ -f "$output" ]; then
    		echo "    File already exists."
    	else
            cmd_ord="./ord $input gorder -d -o $rank -n $window"
            echo "    $cmd_ord"
            $cmd_ord
            echo
            cmd_rank="./rankedges $input $rank $output"
            echo "    $cmd_rank"
            $cmd_rank

            rm $rank
        fi

    	file=$dirr/window-${dataset_name}-${algo}-${window}.${loop}.txt
    	# echo "    ${file}"

    	if [ -f "$file" ]; then
    		echo "    File already exists."
            cat $file
    	else
            cmd_benchmark="./benchmark $output -a $algo -o $file -l 10"
            echo "    $cmd_benchmark"
            $cmd_benchmark
            echo
    	fi

        echo

        window=$((window * 2))
    done
done

# ----------------------------------------
# -------- call to plot functions --------
# ----------------------------------------
echo "Creating figures for window tuning"
python3 $python_draw $dataset_name $algo
