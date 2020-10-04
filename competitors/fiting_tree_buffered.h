#pragma once

#include "../util.h"
#include "base.h"
#include "competitors/FITing-Tree/include/fiting_tree/buffered_fiting_tree.h"

#define ValueType uint64_t

template <class KeyType, uint16_t Error, uint16_t BufferSize = 32>
class BufferedFITingTree : public Competitor {

public:
  uint64_t Build(const std::vector<KeyValue<KeyType>> &data) {
      const auto extract_key
      = [](KeyValue<KeyType> kv) { return kv.key; };

      std::vector<KeyType> keys;
      keys.reserve(data.size());
      std::transform(data.begin(), data.end(),
                  std::back_inserter(keys),
                  extract_key);

      return util::timing([&] {
          buffered_fiting_tree_ = BufferedFitingTree<KeyType, ValueType, Error, BufferSize>(keys);
      });
  }

  SearchBound EqualityLookup(const KeyType lookup_key) const {
      const auto it = buffered_fiting_tree_.find(lookup_key);
      return (SearchBound) {it->pos(), it->pos()};
  }

  template<typename KT>
  uint64_t Insert(const std::vector<KeyValue<KT>> &data) {
    
    uint64_t timing_sum = 0, timing;
    for (auto kv : data) {
      timing = util::timing([&] {
        buffered_fiting_tree_.insert(kv.key, kv.value);
      });
      timing_sum += timing;
    }
    
    return timing_sum;
  }

  std::string name() const {
      return "BufferedFITing";
  }

  std::size_t size() const {
      return 0;
  }

  int variant() const {
      return Error;
  }

  bool insertion_possible() const {
    return true;
  }

private:
    BufferedFitingTree<KeyType, ValueType, Error, BufferSize> buffered_fiting_tree_;
};
