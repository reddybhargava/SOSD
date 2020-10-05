#include "benchmarks/common.h"
#include "benchmarks/benchmark_fitingtreebuffered.h"
#include "benchmark.h"
#include "competitors/fiting_tree_buffered.h"

template<template<typename> typename Searcher>
void benchmark_32_bufferedfitingtree(sosd::Benchmark<uint32_t, Searcher> &benchmark, bool pareto) {
  benchmark.template Run<BufferedFITingTree<uint32_t, 128, 96>>();
  benchmark.template Run<BufferedFITingTree<uint32_t, 64, 32>>();
  if (pareto) {
    benchmark.template Run<BufferedFITingTree<uint32_t, 16, 8>>();
    benchmark.template Run<BufferedFITingTree<uint32_t, 20, 10>>();
    benchmark.template Run<BufferedFITingTree<uint32_t, 32, 16>>();
    benchmark.template Run<BufferedFITingTree<uint32_t, 50, 40>>();
    benchmark.template Run<BufferedFITingTree<uint32_t, 100, 50>>();
    benchmark.template Run<BufferedFITingTree<uint32_t, 150, 140>>();
  }
}

template<template<typename> typename Searcher>
void benchmark_64_bufferedfitingtree(sosd::Benchmark<uint64_t, Searcher> &benchmark, bool pareto) {
  benchmark.template Run<BufferedFITingTree<uint64_t, 128, 96>>();
  benchmark.template Run<BufferedFITingTree<uint64_t, 64, 32>>();
  if (pareto) {
    benchmark.template Run<BufferedFITingTree<uint64_t, 16, 8>>();
    benchmark.template Run<BufferedFITingTree<uint64_t, 20, 10>>();
    benchmark.template Run<BufferedFITingTree<uint64_t, 32, 16>>();
    benchmark.template Run<BufferedFITingTree<uint64_t, 50, 40>>();
    benchmark.template Run<BufferedFITingTree<uint64_t, 100, 50>>();
    benchmark.template Run<BufferedFITingTree<uint64_t, 150, 140>>();
  }
}

INSTANTIATE_TEMPLATES(benchmark_32_bufferedfitingtree, uint32_t);
INSTANTIATE_TEMPLATES(benchmark_64_bufferedfitingtree, uint64_t);
