#pragma once

#include "../util.h"

class Competitor {
 public:
  bool applicable(bool _unique, const std::string& data_filename) const {
    return true;
  }

  int variant() const {
    return 0;
  }

  bool insertion_possible() const {
    return false;
  }

  template<typename KeyType>
  uint64_t Insert(const std::vector<KeyValue<KeyType>> &data) {
    return 0;
  }
};
