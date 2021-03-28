#! /usr/bin/env bash

echo "Executing benchmark and saving results..."

BENCHMARK=build/benchmark
if [ ! -f $BENCHMARK ]; then
    echo "benchmark binary does not exist"
    exit
fi

function do_benchmark() {

    RESULTS=./results/$1_cold_repetition_results_$2.txt
    if [ -f $RESULTS ]; then
        echo "Already have results for $1 ($2)"
    else
        echo "Executing workload $1 ($2)"
        $BENCHMARK -r 1 ./data/$1 ./data/$1_equality_lookups_repetitions_$2 --pareto --cold-cache | tee ./results/$1_cold_repetition_results_$2.txt
    fi
}

mkdir -p ./results

for dataset in $(cat scripts/datasets_under_test.txt); do
    do_benchmark "$dataset" "120K"
    do_benchmark "$dataset" "80K"
    do_benchmark "$dataset" "60K"
done
