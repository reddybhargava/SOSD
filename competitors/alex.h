#pragma once

#include "../util.h"
#include "base.h"
#include "competitors/ALEX/src/core/alex.h"

#define ValueType uint64_t

template <class KeyType, int VariantNumber>
class ALEX : public Competitor {

public:
  uint64_t Build(const std::vector<KeyValue<KeyType>> &data) {
    int num_keys = data.size();
    std::pair<KeyType, ValueType> *bulk_data = new std::pair<KeyType, ValueType>[num_keys];

    for (int pos = 0; pos < num_keys; pos++)
      bulk_data[pos] = {data[pos].key, pos};

    return util::timing([&] {
      alex_.bulk_load(bulk_data, num_keys);
    });
  }

  SearchBound EqualityLookup(const KeyType lookup_key) const {
    size_t start = 0, stop = 0;
    if (VariantNumber == 1) {
      auto pos = alex_.find(lookup_key);
      start = (*pos).second, stop = (*pos).second;
    }
    else if (VariantNumber == 2) {
      auto lb = alex_.lower_bound(lookup_key), ub = alex_.upper_bound(lookup_key);
      start = (*lb).second, stop = (*ub).second;
    }

    return (SearchBound){start, stop};
  }

  std::string name() const {
    return "ALEX";
  }

  std::size_t size() const {
    // const size_t total_size = alex_.model_size() + alex_.data_size();
    return 0;
  }

  int variant() const {
    return VariantNumber;
  }

private:
  alex::Alex<KeyType, ValueType> alex_;
};
