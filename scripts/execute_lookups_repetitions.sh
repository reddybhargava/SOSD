#! /usr/bin/env bash
#         Usage: scripts/execute_lookups_zipf.sh [perf|cold|normal]
# Example Usage: scripts/execute_lookups_zipf.sh cold


echo "Executing benchmark and saving results..."
BENCHMARK=build/benchmark
if [ ! -f $BENCHMARK ]; then
    echo "benchmark binary does not exist"
    exit
fi

function do_benchmark() {
    case $1 in
        "perf") 
            perf $2 $3
        ;;
        "cold") 
            cold $2 $3
        ;;
        "normal")
            normal $2 $3
        ;;
    esac
}

function perf() {
    RESULTS=./results/$1_perf_repetitions_results_$2.txt
    if [ -f $RESULTS ]; then
        echo "Already have results for $1 ($2)"
    else
        echo "Executing workload $1 ($2)"
        $BENCHMARK -r 1 --perf --pareto ./data/$1 ./data/$1_equality_lookups_repetitions_$2 | tee ./results/$1_perf_repetitions_results_$2.txt
    fi
}

function cold() {
    RESULTS=./results/$1_cold_repetitions_results_$2.txt
    if [ -f $RESULTS ]; then
        echo "Already have results for $1 ($2)"
    else
        echo "Executing workload $1 ($2)"
        $BENCHMARK -r 1 --cold-cache --pareto ./data/$1 ./data/$1_equality_lookups_repetitions_$2 | tee ./results/$1_cold_repetitions_results_$2.txt
    fi
}

function normal() {
    RESULTS=./results/$1_normal_repetitions_results_$2.txt
    if [ -f $RESULTS ]; then
        echo "Already have results for $1 ($2)"
    else
        echo "Executing workload $1 ($2)"
        $BENCHMARK -r 1 --pareto ./data/$1 ./data/$1_equality_lookups_repetitions_$2 | tee ./results/$1_normal_repetitions_results_$2.txt
    fi
}

mkdir -p ./results

for dataset in $(cat scripts/datasets_under_test.txt); do
    # do_benchmark $1 "$dataset" "1010K"
    # do_benchmark $1 "$dataset" "1050K"
    do_benchmark $1 "$dataset" "1100K"
    do_benchmark $1 "$dataset" "1500K"
    # do_benchmark $1 "$dataset" "1750K"
    do_benchmark $1 "$dataset" "2M"
done
