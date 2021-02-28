#! /usr/bin/env bash
set -e

echo "Compiling benchmark..."
git submodule update --init --recursive 

mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j 8 

function generate_lookups() {
    echo "Generating lookups for $1"
    [ -f ../data/$1_equality_lookups_10M ] || ./generate ../data/$1 10000000
    [ -f ../data/$1_equality_lookups_1M ] || ./generate ../data/$1 1000000
}

function generate_inserts() {
    echo "Generating inserts for $1"
    generate_inserts_files $1
}

function generate_inserts_files() {
    [ -f ../data/$1_inserts_10M ] || ./generate ../data/$1 10000000 1
    [ -f ../data/$1_inserts_1M ] || ./generate ../data/$1 1000000 1
} &> /dev/null

function generate_lookups_repetitions() {
    echo "Generating lookups with repetition for $1 ($2)"
    # [ -f ../data/$1_equality_lookups_repetitions_10M ] || ./generate ../data/$1 10000000 0 10000
    [ -f ../data/$1_equality_lookups_repetitions_$2 ] || ./generate ../data/$1 1000000 0 $3 $4 $5
}

# # Generating Lookups
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