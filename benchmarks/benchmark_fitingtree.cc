#include "benchmarks/common.h"
#include "benchmarks/benchmark_fitingtree.h"
#include "benchmark.h"
#include "competitors/fiting_tree.h"

template<template<typename> typename Searcher>
void benchmark_32_fitingtree(sosd::Benchmark<uint32_t, Searcher> &benchmark, bool pareto) {
  benchmark.template Run<FITingTree<uint32_t, 128>>();
  if (pareto) {
    benchmark.template Run<FITingTree<uint32_t, 8>>();
    benchmark.template Run<FITingTree<uint32_t, 16>>();
    benchmark.template Run<FITingTree<uint32_t, 32>>();
    benchmark.template Run<FITingTree<uint32_t, 64>>();
    benchmark.template Run<FITingTree<uint32_t, 256>>();
    benchmark.template Run<FITingTree<uint32_t, 512>>();
    benchmark.template Run<FITingTree<uint32_t, 1024>>();
  }
}

template<template<typename> typename Searcher>
void benchmark_64_fitingtree(sosd::Benchmark<uint64_t, Searcher> &benchmark, bool pareto) {
  benchmark.template Run<FITingTree<uint64_t, 128>>();
  if (pareto) {
    benchmark.template Run<FITingTree<uint64_t, 8>>();
    benchmark.template Run<FITingTree<uint64_t, 16>>();
    benchmark.template Run<FITingTree<uint64_t, 32>>();
    benchmark.template Run<FITingTree<uint64_t, 64>>();
    benchmark.template Run<FITingTree<uint64_t, 256>>();
    benchmark.template Run<FITingTree<uint64_t, 512>>();
    benchmark.template Run<FITingTree<uint64_t, 1024>>();
  }
}

INSTANTIATE_TEMPLATES(benchmark_32_fitingtree, uint32_t);
INSTANTIATE_TEMPLATES(benchmark_64_fitingtree, uint64_t);
