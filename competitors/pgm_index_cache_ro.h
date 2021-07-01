#ifndef SOSDB_PGMCACHERO_H
#define SOSDB_PGMCACHERO_H

#include "base.h"
#include "../util.h"

#include <vector>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include "pgm_index_cache_random_opt.hpp"
//#include <functional>
//#include <boost/iterator/transform_iterator.hpp>
//#include <boost/range/adaptors.hpp>
// #include <valgrind/callgrind.h>

#define ValueType uint64_t

template <class KeyType, int pgm_error>
class PGMCacheRO : public Competitor
{

public:
  uint64_t Build(const std::vector<KeyValue<KeyType>> &data)
  {

    const auto extract_key = [](KeyValue<KeyType> kv) { return kv.key; };

    // This code uses a boost transform iterator to avoid a copy. It
    // seems to be much slower, however.
    /*
    auto it_begin = boost::make_transform_iterator(data.begin(), extract_key);
    auto it_end = boost::make_transform_iterator(data.end(), extract_key);
    pgm_ = PGMIndex<KeyType, pgm_error, 4>(it_begin, it_end);
    */

    // don't count the data copy time against the PGM build time
    std::vector<KeyType> keys;
    keys.reserve(data.size());
    std::transform(data.begin(), data.end(),
                   std::back_inserter(keys),
                   extract_key);

    uint64_t build_time = util::timing([&] {
      pgm_ = decltype(pgm_)(keys.begin(), keys.end());
    });
    return build_time;
  }

  SearchBound EqualityLookup(const KeyType lookup_key)
  {
    // CALLGRIND_START_INSTRUMENTATION;
    // CALLGRIND_TOGGLE_COLLECT;
    auto approx_range = pgm_.find_approximate_position(lookup_key);
    // CALLGRIND_TOGGLE_COLLECT;
    // CALLGRIND_STOP_INSTRUMENTATION;

    auto lo = approx_range.lo;
    auto hi = approx_range.hi;

    if (lo == hi)
      return (SearchBound){lo, hi};
    else
      return (SearchBound){lo, hi + 1};
  }

  template <typename KT>
  void WriteCache(const KT key, const ValueType position)
  {
    pgm_.add_cache_pos(key, position);
  }

  std::string name() const
  {
    return "PGMCacheRO (" + std::to_string(pgm_.cacheHits) + ", " + std::to_string(pgm_.cacheTime) + ", " + std::to_string(pgm_.count) + ")";
  }

  std::size_t size() const
  {
    return pgm_.size_in_bytes();
  }

  bool applicable(bool unique, const std::string &data_filename) const
  {
    return true;
  }

  int variant() const { return pgm_error; }

private:
  PGMIndex<KeyType, pgm_error, 4> pgm_;
};

#endif //SOSDB_PGM_H