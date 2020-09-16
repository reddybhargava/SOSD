#pragma once

#include "../util.h"
#include "base.h"
#include "competitors/FITing-Tree/include/fiting_tree/fiting_tree.h"


template <class KeyType, uint16_t Error>
class FITingTree : public Competitor {

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
          fiting_tree_ = FitingTree<KeyType, Error>(keys);
      });
  }

  SearchBound EqualityLookup(const KeyType lookup_key) const {
      const auto pos = fiting_tree_.get_approx_pos(lookup_key);
      return (SearchBound) {pos.lo, pos.hi};
  }

  std::string name() const {
      return "FITing";
  }

  std::size_t size() const {
      return 0;
  }

  int variant() const {
      return Error;
  }

private:
    FitingTree<KeyType, Error> fiting_tree_;
};
