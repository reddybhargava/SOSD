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
