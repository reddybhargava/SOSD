#ifndef SOSDB_PGM_H
#define SOSDB_PGM_H

#include "base.h"
#include "../util.h"

#include <vector>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include "pgm_index.hpp"
#include "pgm_index_dynamic.hpp"
//#include <functional>
//#include <boost/iterator/transform_iterator.hpp>
//#include <boost/range/adaptors.hpp>


#define ValueType uint64_t

template<class KeyType, int pgm_error>
class PGM : public Competitor {

public:
  uint64_t Build(const std::vector<KeyValue<KeyType>>& data) {
    
    const auto extract_key
      = [](KeyValue<KeyType> kv) { return kv.key; };

    std::vector<KeyType> keys;
    keys.reserve(data.size());
    std::transform(data.begin(), data.end(),
                   std::back_inserter(keys),
                   extract_key);

    pgm_ = decltype(pgm_)(keys.begin(), keys.end());
    keys.clear();

    // Dynamic PGM
    const auto d_extract_key = [](KeyValue<KeyType> kv) { 
        auto key = kv.key, value = kv.value;
        return std::make_pair(key, value); 
    };

    // don't count the data copy time against the PGM build time
    std::vector<std::pair<KeyType, ValueType>> d_keys;
    keys.reserve(data.size());
    std::transform(data.begin(), data.end(),
                   std::back_inserter(d_keys),
                   d_extract_key);

    uint64_t build_time = util::timing([&] {
      dpgm_ = decltype(dpgm_)(d_keys.begin(), d_keys.end());
    });

    return build_time;
  }

  SearchBound EqualityLookup(const KeyType lookup_key) const {
    auto approx_range = pgm_.find_approximate_position(lookup_key);
    auto lo = approx_range.lo;
    auto hi = approx_range.hi;

    return (SearchBound){ lo, hi + 1 };

  }

  template<typename KT>
  uint64_t Insert(const std::vector<KeyValue<KT>> &data) {
    
    uint64_t timing_sum = 0, timing;
    for (auto kv : data) {
      timing = util::timing([&] {
        dpgm_.insert(kv.key, kv.value);
      });
      timing_sum += timing;
    }
    
    return timing_sum;
  }

  std::string name() const {
    return "DPGM";
  }

  std::size_t size() const {
    return pgm_.size_in_bytes();
  }

  bool applicable(bool unique, const std::string& data_filename) const {
    return true;
  }

  int variant() const { return pgm_error; }

  bool insertion_possible() const {
    return true;
  }

private:
  PGMIndex<KeyType, pgm_error, 4> pgm_;
  DynamicPGMIndex<KeyType, ValueType, PGMIndex<KeyType, pgm_error, 4>> dpgm_;
};

#endif //SOSDB_PGM_H
