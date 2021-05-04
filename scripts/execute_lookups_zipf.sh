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
            perf $2 $3 $4
        ;;
        "cold") 
            cold $2 $3 $4
        ;;
        "normal")
            normal $2 $3 $4
        ;;
    esac
}

function perf() {
    RESULTS=./results/$1_perf_zipf_results_$2_$3.txt
    if [ -f $RESULTS ]; then
        echo "Already have results for $1 ($2, $3)"
    else
        echo "Executing workload $1 ($2, $3)"
        $BENCHMARK -r 1 --perf --pareto ./data/$1 ./data/$1_equality_lookups_zipf_$2_$3 | tee ./results/$1_perf_zipf_results_$2_$3.txt
    fi
}

function cold() {
    RESULTS=./results/$1_cold_zipf_results_$2_$3.txt
    if [ -f $RESULTS ]; then
        echo "Already have results for $1 ($2, $3)"
    else
        echo "Executing workload $1 ($2, $3)"
        $BENCHMARK -r 1 --cold-cache --pareto ./data/$1 ./data/$1_equality_lookups_zipf_$2_$3 | tee ./results/$1_cold_zipf_results_$2_$3.txt
    fi
}

function normal() {
    RESULTS=./results/$1_normal_zipf_results_$2_$3.txt
    if [ -f $RESULTS ]; then
        echo "Already have results for $1 ($2, $3)"
    else
        echo "Executing workload $1 ($2, $3)"
        $BENCHMARK -r 1 --pareto ./data/$1 ./data/$1_equality_lookups_zipf_$2_$3 | tee ./results/$1_normal_zipf_results_$2_$3.txt
    fi
}

mkdir -p ./results

for dataset in $(cat scripts/datasets_under_test.txt); do
#     do_benchmark $1 "$dataset" "1K" "1"
#     do_benchmark $1 "$dataset" "1K" "2"
#     do_benchmark $1 "$dataset" "1K" "3"

#    do_benchmark $1 "$dataset" "10K" "1"
#    do_benchmark $1 "$dataset" "10K" "2"
#    do_benchmark $1 "$dataset" "10K" "3"

#    do_benchmark $1 "$dataset" "50K" "1"
#    do_benchmark $1 "$dataset" "50K" "2"
#    do_benchmark $1 "$dataset" "50K" "3"
    
#    do_benchmark $1 "$dataset" "100K" "1"
#    do_benchmark $1 "$dataset" "100K" "2"
#    do_benchmark $1 "$dataset" "100K" "3"

#    do_benchmark $1 "$dataset" "500K" "1"
#    do_benchmark $1 "$dataset" "500K" "2"
   do_benchmark $1 "$dataset" "500K" "3"

#    do_benchmark $1 "$dataset" "1M" "1"
#    do_benchmark $1 "$dataset" "1M" "2"
#    do_benchmark $1 "$dataset" "1M" "3"
done
