#! /usr/bin/env bash

echo "Executing benchmark and saving results..."

BENCHMARK=build/benchmark
if [ ! -f $BENCHMARK ]; then
    echo "benchmark binary does not exist"
    exit
fi

function do_benchmark() {

    RESULTS=./results/$1_zipf_results_random_$2_$3.txt
    if [ -f $RESULTS ]; then
        echo "Already have results for $1 ($2, $3)"
    else
        echo "Executing workload $1 ($2, $3)"
        $BENCHMARK -r 1 ./data/$1 ./data/$1_equality_lookups_zipf_$2_$3  --pareto | tee ./results/$1_zipf_results_random_$2_$3.txt
    fi
}

mkdir -p ./results

for dataset in $(cat scripts/datasets_under_test.txt); do
    do_benchmark "$dataset" "1K" "1"
    do_benchmark "$dataset" "1K" "2"
    do_benchmark "$dataset" "1K" "3"

    do_benchmark "$dataset" "10K" "1"
    do_benchmark "$dataset" "10K" "2"
    do_benchmark "$dataset" "10K" "3"

#    do_benchmark "$dataset" "50K" "1"
#    do_benchmark "$dataset" "50K" "2"
#    do_benchmark "$dataset" "50K" "3"
    
    do_benchmark "$dataset" "100K" "1"
    do_benchmark "$dataset" "100K" "2"
    do_benchmark "$dataset" "100K" "3"

    # do_benchmark "$dataset" "500K" "1"
    # do_benchmark "$dataset" "500K" "2"
    # do_benchmark "$dataset" "500K" "3"

    # do_benchmark "$dataset" "1M" "1"
    # do_benchmark "$dataset" "1M" "2"
    # do_benchmark "$dataset" "1M" "3"
done
