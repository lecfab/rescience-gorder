#!/bin/bash
# comparison of resulting energy for different parameters of simulated annealing

dataset=edgelist-epinion-75k-508k

dirr=../results/annealing
html=../results/gorder-annealing.html

mkdir $dirr
echo "Results will be found in $dirr "
cat run-annealing.sh > $dirr/backup-run.sh
cat $html > $dirr/backup-html.html

./parametrise ../datasets/${dataset}.txt minla -o ../results/annealing/tune-minla.js
./parametrise ../datasets/${dataset}.txt minloga -o ../results/annealing/tune-minloga.js

echo

if command -v firefox &> /dev/null; then
    echo "File $html is open in Firefox."
    firefox $html
else
    echo "Open file $html in a web browser to see the plot."
fi
