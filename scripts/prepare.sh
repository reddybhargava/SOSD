#! /usr/bin/env bash
set -e

echo "Compiling benchmark..."
git submodule update --init --recursive 

mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j 8 

function generate_lookups() {
    echo "generate_lookups(): Generating lookups for $1"
#    [ -f ../data/$1_equality_lookups_10M ] || ./generate ../data/$1 10000000
    [ -f ../data/$1_equality_lookups_1M ] || ./generate ../data/$1 1000000
}

function generate_inserts() {
    echo "generate_inserts(): Generating inserts for $1"
    generate_inserts_files $1
}

function generate_inserts_files() {
    [ -f ../data/$1_inserts_10M ] || ./generate ../data/$1 10000000 1
    [ -f ../data/$1_inserts_1M ] || ./generate ../data/$1 1000000 1
} &> /dev/null

function generate_lookups_repetitions() {
    echo "generate_lookups_repetitions(): Generating lookups with repetition for $1 ($2)"
    [ -f ../data/$1_equality_lookups_repetitions_$2 ] || ./generate ../data/$1 1000000 0 $3 $4 $5
}

function generate_lookups_cold() {
    echo "generate_lookups_cold(): Generating lookups for $1"
    [ -f ../data/$1_equality_lookups_$2 ] || ./generate ../data/$1 $3
}

function generate_lookups_repetitions_cold() {
    echo "generate_lookups_repetitions_cold(): Generating lookups for $1 ($2)"
    [ -f ../data/$1_equality_lookups_repetitions_$2 ] || ./generate ../data/$1 $3 0 $4 $5 $6
}

function generate_lookups_zipf() {
    echo "generate_lookups_zipf(): Generating lookups for $1 ($2)"
    [ -f ../data/$1_equality_lookups_zipf_$2_1 ] || ./generate ../data/$1 $3 0 0 0 0 1 1
    [ -f ../data/$1_equality_lookups_zipf_$2_2 ] || ./generate ../data/$1 $3 0 0 0 0 1 2
    [ -f ../data/$1_equality_lookups_zipf_$2_3 ] || ./generate ../data/$1 $3 0 0 0 0 1 3
}

# # Generating Lookups
echo "Generating queries..."
generate_lookups normal_200M_uint32
generate_lookups normal_200M_uint64

generate_lookups lognormal_200M_uint32
generate_lookups lognormal_200M_uint64

generate_lookups uniform_dense_200M_uint32
generate_lookups uniform_dense_200M_uint64

generate_lookups uniform_sparse_200M_uint32
generate_lookups uniform_sparse_200M_uint64

generate_lookups osm_cellids_200M_uint64
# generate_lookups osm_cellids_400M_uint64
# generate_lookups osm_cellids_600M_uint64
# generate_lookups osm_cellids_800M_uint64

generate_lookups wiki_ts_200M_uint64

generate_lookups books_200M_uint32
generate_lookups books_200M_uint64
# generate_lookups books_400M_uint64
# generate_lookups books_600M_uint64
# generate_lookups books_800M_uint64

generate_lookups fb_200M_uint64

# # Generating Inserts
generate_inserts osm_cellids_200M_uint64
generate_inserts wiki_ts_200M_uint64
generate_inserts books_200M_uint32
generate_inserts books_200M_uint64
generate_inserts fb_200M_uint64


# # Generating Lookups with Repetitions
generate_lookups_repetitions osm_cellids_200M_uint64 "1010K" 10000 0 100
generate_lookups_repetitions wiki_ts_200M_uint64 "1010K" 10000 0 100
generate_lookups_repetitions books_200M_uint32 "1010K" 10000 0 100
generate_lookups_repetitions books_200M_uint64 "1010K" 10000 0 100
generate_lookups_repetitions fb_200M_uint64 "1010K" 10000 0 100

# # Generating Lookups with Repetitions
generate_lookups_repetitions osm_cellids_200M_uint64 "1050K" 50000 0 1000
generate_lookups_repetitions wiki_ts_200M_uint64 "1050K" 50000 0 1000
generate_lookups_repetitions books_200M_uint32 "1050K" 50000 0 1000
generate_lookups_repetitions books_200M_uint64 "1050K" 50000 0 1000
generate_lookups_repetitions fb_200M_uint64 "1050K" 50000 0 1000

# # Generating Lookups with Repetitions
generate_lookups_repetitions osm_cellids_200M_uint64 "1100K" 100000 0 5000
generate_lookups_repetitions wiki_ts_200M_uint64 "1100K" 100000 0 5000
generate_lookups_repetitions books_200M_uint32 "1100K" 100000 0 5000
generate_lookups_repetitions books_200M_uint64 "1100K" 100000 0 5000
generate_lookups_repetitions fb_200M_uint64 "1100K" 100000 0 5000

# # Generating Lookups with Repetitions
generate_lookups_repetitions osm_cellids_200M_uint64 "1500K" 500000 0 5000
generate_lookups_repetitions wiki_ts_200M_uint64 "1500K" 500000 0 5000
generate_lookups_repetitions books_200M_uint32 "1500K" 500000 0 5000
generate_lookups_repetitions books_200M_uint64 "1500K" 500000 0 5000
generate_lookups_repetitions fb_200M_uint64 "1500K" 500000 0 5000

# # Generating Lookups with Repetitions
generate_lookups_repetitions osm_cellids_200M_uint64 "1750K" 750000 0 10000
generate_lookups_repetitions wiki_ts_200M_uint64 "1750K" 750000 0 10000
generate_lookups_repetitions books_200M_uint32 "1750K" 750000 0 10000
generate_lookups_repetitions books_200M_uint64 "1750K" 750000 0 10000
generate_lookups_repetitions fb_200M_uint64 "1750K" 750000 0 10000

# # Generating Lookups with Repetitions
generate_lookups_repetitions osm_cellids_200M_uint64 "2M" 1000000 1000 100000
generate_lookups_repetitions wiki_ts_200M_uint64 "2M" 1000000 1000 100000
generate_lookups_repetitions books_200M_uint32 "2M" 1000000 1000 100000
generate_lookups_repetitions books_200M_uint64 "2M" 1000000 1000 100000
generate_lookups_repetitions fb_200M_uint64 "2M" 1000000 1000 100000


# # Generating Lookups for cold 
generate_lookups_cold osm_cellids_200M_uint64 "20K" 20000
generate_lookups_cold wiki_ts_200M_uint64 "20K" 20000
generate_lookups_cold books_200M_uint32 "20K" 20000
generate_lookups_cold books_200M_uint64 "20K" 20000
generate_lookups_cold fb_200M_uint64 "20K" 20000

# # Generating Lookups for cold 
generate_lookups_cold osm_cellids_200M_uint64 "50K" 50000
generate_lookups_cold wiki_ts_200M_uint64 "50K" 50000
generate_lookups_cold books_200M_uint32 "50K" 50000
generate_lookups_cold books_200M_uint64 "50K" 50000
generate_lookups_cold fb_200M_uint64 "50K" 50000


# # Generating Lookups for cold with Repetition
generate_lookups_repetitions_cold osm_cellids_200M_uint64 "21K" 20000 1000 0 100
generate_lookups_repetitions_cold wiki_ts_200M_uint64 "21K" 20000 1000 0 100
generate_lookups_repetitions_cold books_200M_uint32 "21K" 20000 1000 0 100
generate_lookups_repetitions_cold books_200M_uint64 "21K" 20000 1000 0 100
generate_lookups_repetitions_cold fb_200M_uint64 "21K" 20000 1000 0 100

# # Generating Lookups for cold with Repetition
generate_lookups_repetitions_cold osm_cellids_200M_uint64 "25K" 20000 5000 0 200
generate_lookups_repetitions_cold wiki_ts_200M_uint64 "25K" 20000 5000 0 200
generate_lookups_repetitions_cold books_200M_uint32 "25K" 20000 5000 0 200
generate_lookups_repetitions_cold books_200M_uint64 "25K" 20000 5000 0 200
generate_lookups_repetitions_cold fb_200M_uint64 "25K" 20000 5000 0 200

# # Generating Lookups for cold with Repetition
generate_lookups_repetitions_cold osm_cellids_200M_uint64 "30K" 20000 10000 0 500
generate_lookups_repetitions_cold wiki_ts_200M_uint64 "30K" 20000 10000 0 500
generate_lookups_repetitions_cold books_200M_uint32 "30K" 20000 10000 0 500
generate_lookups_repetitions_cold books_200M_uint64 "30K" 20000 10000 0 500
generate_lookups_repetitions_cold fb_200M_uint64 "30K" 20000 10000 0 500

# # Generating Lookups for cold with Repetition
generate_lookups_repetitions_cold osm_cellids_200M_uint64 "40K" 20000 20000 0 1000
generate_lookups_repetitions_cold wiki_ts_200M_uint64 "40K" 20000 20000 0 1000
generate_lookups_repetitions_cold books_200M_uint32 "40K" 20000 20000 0 1000
generate_lookups_repetitions_cold books_200M_uint64 "40K" 20000 20000 0 1000
generate_lookups_repetitions_cold fb_200M_uint64 "40K" 20000 20000 0 1000

# # Generating Lookups for cold with Repetition
generate_lookups_repetitions_cold osm_cellids_200M_uint64 "60K" 20000 40000 0 1000
generate_lookups_repetitions_cold wiki_ts_200M_uint64 "60K" 20000 40000 0 1000
generate_lookups_repetitions_cold books_200M_uint32 "60K" 20000 40000 0 1000
generate_lookups_repetitions_cold books_200M_uint64 "60K" 20000 40000 0 1000
generate_lookups_repetitions_cold fb_200M_uint64 "60K" 20000 40000 0 1000

# # Generating Lookups for cold with Repetition
generate_lookups_repetitions_cold osm_cellids_200M_uint64 "80K" 20000 60000 0 1000
generate_lookups_repetitions_cold wiki_ts_200M_uint64 "80K" 20000 60000 0 1000
generate_lookups_repetitions_cold books_200M_uint32 "80K" 20000 60000 0 1000
generate_lookups_repetitions_cold books_200M_uint64 "80K" 20000 60000 0 1000
generate_lookups_repetitions_cold fb_200M_uint64 "80K" 20000 60000 0 1000

# # Generating Lookups for cold with Repetition
generate_lookups_repetitions_cold osm_cellids_200M_uint64 "120K" 20000 100000 0 1250
generate_lookups_repetitions_cold wiki_ts_200M_uint64 "120K" 20000 100000 0 1250
generate_lookups_repetitions_cold books_200M_uint32 "120K" 20000 100000 0 1250
generate_lookups_repetitions_cold books_200M_uint64 "120K" 20000 100000 0 1250
generate_lookups_repetitions_cold fb_200M_uint64 "120K" 20000 100000 0 1250


# # Generating Lookups for cold with Repetition
generate_lookups_repetitions_cold osm_cellids_200M_uint64 "75K" 50000 25000 0 1000
generate_lookups_repetitions_cold wiki_ts_200M_uint64 "75K" 50000 25000 0 1000
generate_lookups_repetitions_cold books_200M_uint32 "75K" 50000 25000 0 1000
generate_lookups_repetitions_cold books_200M_uint64 "75K" 50000 25000 0 1000
generate_lookups_repetitions_cold fb_200M_uint64 "75K" 50000 25000 0 1000

# # Generating Lookups for cold with Repetition
generate_lookups_repetitions_cold osm_cellids_200M_uint64 "100K" 50000 50000 0 2500
generate_lookups_repetitions_cold wiki_ts_200M_uint64 "100K" 50000 50000 0 2500
generate_lookups_repetitions_cold books_200M_uint32 "100K" 50000 50000 0 2500
generate_lookups_repetitions_cold books_200M_uint64 "100K" 50000 50000 0 2500
generate_lookups_repetitions_cold fb_200M_uint64 "100K" 50000 50000 0 2500


# # Generating Lookups with Zipf Distributions
generate_lookups_zipf osm_cellids_200M_uint64 "1K" 1000
generate_lookups_zipf wiki_ts_200M_uint64 "1K" 1000
generate_lookups_zipf books_200M_uint32 "1K" 1000
generate_lookups_zipf books_200M_uint64 "1K" 1000
generate_lookups_zipf fb_200M_uint64 "1K" 1000

# # Generating Lookups with Zipf Distributions
generate_lookups_zipf osm_cellids_200M_uint64 "10K" 10000
generate_lookups_zipf wiki_ts_200M_uint64 "10K" 10000
generate_lookups_zipf books_200M_uint32 "10K" 10000
generate_lookups_zipf books_200M_uint64 "10K" 10000
generate_lookups_zipf fb_200M_uint64 "10K" 10000

# # Generating Lookups with Zipf Distributions
generate_lookups_zipf osm_cellids_200M_uint64 "50K" 50000
generate_lookups_zipf wiki_ts_200M_uint64 "50K" 50000
generate_lookups_zipf books_200M_uint32 "50K" 50000
generate_lookups_zipf books_200M_uint64 "50K" 50000
generate_lookups_zipf fb_200M_uint64 "50K" 50000

# # Generating Lookups with Zipf Distributions
generate_lookups_zipf osm_cellids_200M_uint64 "100K" 100000
generate_lookups_zipf wiki_ts_200M_uint64 "100K" 100000
generate_lookups_zipf books_200M_uint32 "100K" 100000
generate_lookups_zipf books_200M_uint64 "100K" 100000
generate_lookups_zipf fb_200M_uint64 "100K" 100000

# # Generating Lookups with Zipf Distributions
generate_lookups_zipf osm_cellids_200M_uint64 "500K" 500000
generate_lookups_zipf wiki_ts_200M_uint64 "500K" 500000
generate_lookups_zipf books_200M_uint32 "500K" 500000
generate_lookups_zipf books_200M_uint64 "500K" 500000
generate_lookups_zipf fb_200M_uint64 "500K" 500000

# # Generating Lookups with Zipf Distributions
generate_lookups_zipf osm_cellids_200M_uint64 "1M" 1000000
generate_lookups_zipf wiki_ts_200M_uint64 "1M" 1000000
generate_lookups_zipf books_200M_uint32 "1M" 1000000
generate_lookups_zipf books_200M_uint64 "1M" 1000000
generate_lookups_zipf fb_200M_uint64 "1M" 1000000
