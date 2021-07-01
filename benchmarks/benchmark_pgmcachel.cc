#include "benchmarks/common.h"
#include "benchmark.h"
#include "benchmarks/benchmark_pgmcachel.h"
#include "competitors/pgm_index_cache_l.h"

template <template <typename> typename Searcher>
void benchmark_32_pgmcache_l(sosd::Benchmark<uint32_t, Searcher> &benchmark, bool pareto)
{
  benchmark.template Run<PGMCacheL<uint32_t, 16>>();
  if (pareto)
  {
    benchmark.template Run<PGMCacheL<uint32_t, 4>>();
    benchmark.template Run<PGMCacheL<uint32_t, 8>>();
    benchmark.template Run<PGMCacheL<uint32_t, 32>>();
    benchmark.template Run<PGMCacheL<uint32_t, 64>>();
    benchmark.template Run<PGMCacheL<uint32_t, 256>>();
    benchmark.template Run<PGMCacheL<uint32_t, 1024>>();
    benchmark.template Run<PGMCacheL<uint32_t, 2048>>();
    if (benchmark.uses_binary_search())
    {
      benchmark.template Run<PGMCacheL<uint32_t, 4096>>();
      benchmark.template Run<PGMCacheL<uint32_t, 8192>>();
    }
  }
}

template <template <typename> typename Searcher>
void benchmark_64_pgmcache_l(sosd::Benchmark<uint64_t, Searcher> &benchmark, bool pareto)
{
  benchmark.template Run<PGMCacheL<uint64_t, 16>>();
  if (pareto)
  {
    benchmark.template Run<PGMCacheL<uint64_t, 4>>();
    benchmark.template Run<PGMCacheL<uint64_t, 8>>();
    benchmark.template Run<PGMCacheL<uint64_t, 32>>();
    benchmark.template Run<PGMCacheL<uint64_t, 64>>();
    benchmark.template Run<PGMCacheL<uint64_t, 256>>();
    benchmark.template Run<PGMCacheL<uint64_t, 1024>>();
    benchmark.template Run<PGMCacheL<uint64_t, 2048>>();
    if (benchmark.uses_binary_search())
    {
      benchmark.template Run<PGMCacheL<uint64_t, 4096>>();
      benchmark.template Run<PGMCacheL<uint64_t, 8192>>();
    }
  }
}

INSTANTIATE_TEMPLATES(benchmark_32_pgmcache_l, uint32_t);
INSTANTIATE_TEMPLATES(benchmark_64_pgmcache_l, uint64_t);
