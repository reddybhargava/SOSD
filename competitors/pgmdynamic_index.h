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


#define ValueType uint64_t

template<class KeyType, int pgm_error>
class DPGM : public Competitor {

public:
  uint64_t Build(const std::vector<KeyValue<KeyType>>& data) {

    const auto extract_key = [](KeyValue<KeyType> kv) { 
        auto key = kv.key, value = kv.value;
        return std::make_pair(key, value); 
    };

    // don't count the data copy time against the PGM build time
    std::vector<std::pair<KeyType, ValueType>> keys;
    keys.reserve(data.size());
    std::transform(data.begin(), data.end(),
                   std::back_inserter(keys),
                   extract_key);

    uint64_t build_time = util::timing([&] {
      dpgm_ = decltype(dpgm_)(keys.begin(), keys.end());
    });

    return build_time;
  }

  SearchBound EqualityLookup(const KeyType lookup_key) const {
    auto pos = dpgm_.find(lookup_key);
    auto lo = pos->value(), hi = pos->value();

    return (SearchBound){ lo, hi };
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
    return dpgm_.size_in_bytes();
  }

  bool applicable(bool unique, const std::string& data_filename) const {
    return true;
  }

  int variant() const { return pgm_error; }

  bool insertion_possible() const {
    return true;
  }

private:
  DynamicPGMIndex<KeyType, ValueType, PGMIndex<KeyType, pgm_error, 4>> dpgm_;
};

#endif //SOSDB_PGM_H
