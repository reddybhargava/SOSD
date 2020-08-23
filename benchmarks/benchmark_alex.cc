#include "benchmarks/common.h"
#include "benchmarks/benchmark_alex.h"
#include "benchmark.h"
#include "competitors/alex.h"

template<template<typename> typename Searcher>
void benchmark_32_alex(sosd::Benchmark<uint32_t, Searcher> &benchmark, bool pareto) {
  benchmark.template Run<ALEX<uint32_t, 1>>();
  if (pareto) {
    benchmark.template Run<ALEX<uint32_t, 2>>();
  }
}

template<template<typename> typename Searcher>
void benchmark_64_alex(sosd::Benchmark<uint64_t, Searcher> &benchmark, bool pareto) {
  benchmark.template Run<ALEX<uint64_t, 1>>();
  if (pareto) {
    benchmark.template Run<ALEX<uint64_t, 2>>();
  }
}

INSTANTIATE_TEMPLATES(benchmark_32_alex, uint32_t);
INSTANTIATE_TEMPLATES(benchmark_64_alex, uint64_t);
