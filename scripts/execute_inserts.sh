#! /usr/bin/env bash

echo "Executing benchmark and saving results..."

BENCHMARK=build/benchmark
if [ ! -f $BENCHMARK ]; then
    echo "benchmark binary does not exist"
    exit
fi

function do_benchmark() {

    RESULTS=./results/$1_results_$2_inserts.txt
    if [ -f $RESULTS ]; then
        echo "Already have results for $1"
    else
        echo "Executing workload $1"
        # for index in BTree RMI RS ALEX PGM DPGM FITing BufferedFITing;
        # do
        #     $BENCHMARK -r 1 ./data/$1 ./data/$1_equality_lookups_10M -i ./data/$1_inserts_$2 --pareto --only $index | tee -a ./results/$1_results_$2_inserts.txt
        # done

        $BENCHMARK ./data/$1 ./data/$1_equality_lookups_1M -i ./data/$1_inserts_$2 --pareto | tee ./results/$1_results_$2_inserts.txt
    fi
}

mkdir -p ./results

for dataset in $(cat scripts/datasets_under_test.txt); do
    do_benchmark "$dataset" "1M"
    do_benchmark "$dataset" "10M"
done

dataset=wiki_ts_200M_uint64
for insert_size in 1M 10M; do
    $BENCHMARK ./data/${dataset} ./data/${dataset}_equality_lookups_1M -i ./data/${dataset}_inserts_$insert_size --pareto --only RMI | tee -a ./results/${dataset}_results_$insert_size_inserts.txt
    $BENCHMARK ./data/${dataset} ./data/${dataset}_equality_lookups_1M -i ./data/${dataset}_inserts_$insert_size --pareto --only RS  | tee -a ./results/${dataset}_results_$insert_size_inserts.txt
    $BENCHMARK ./data/${dataset} ./data/${dataset}_equality_lookups_1M -i ./data/${dataset}_inserts_$insert_size --pareto --only PGM | tee -a ./results/${dataset}_results_$insert_size_inserts.txt
    $BENCHMARK ./data/${dataset} ./data/${dataset}_equality_lookups_1M -i ./data/${dataset}_inserts_$insert_size --pareto --only DPGM | tee -a ./results/${dataset}_results_$insert_size_inserts.txt
    $BENCHMARK ./data/${dataset} ./data/${dataset}_equality_lookups_1M -i ./data/${dataset}_inserts_$insert_size --pareto --only FITing | tee -a ./results/${dataset}_results_$insert_size_inserts.txt
    $BENCHMARK ./data/${dataset} ./data/${dataset}_equality_lookups_1M -i ./data/${dataset}_inserts_$insert_size --pareto --only BufferedFITing | tee -a ./results/${dataset}_results_$insert_size_inserts.txt
    $BENCHMARK ./data/${dataset} ./data/${dataset}_equality_lookups_1M -i ./data/${dataset}_inserts_$insert_size --pareto --only BTree | tee -a ./results/${dataset}_results_$insert_size_inserts.txt

    $BENCHMARK ./data/${dataset} ./data/${dataset}_equality_lookups_1M --pareto --only ALEX | tee -a ./results/${dataset}_results_$insert_size_inserts.txt
done