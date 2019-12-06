#ifndef SOSDB_PGM_H
#define SOSDB_PGM_H

#include "base.h"
#include "../util.h"
#include <vector>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include "pgm_index.hpp"

/*
int main(int argc, char **argv) {
    // Generate some random data
    std::vector<uint64_t> dataset = load_data("osm_cellids_200M_uint64");
    std::cout << "Dataset is loaded." << std::endl;

    // Construct the PGM-index
    const int error = 128;
    PGMIndex<uint64_t, error> index(dataset);

    std::cout << "PGM index constructed." << std::endl;
    
    // Query the PGM-index
    auto q = 42;
    auto approx_range = index.find_approximate_position(q);
    auto lo = dataset.cbegin() + approx_range.lo;
    auto hi = dataset.cbegin() + approx_range.hi;
    std::cout << *std::lower_bound(lo, hi, q);

    return 0;
}
*/

template<class KeyType>
class PGM : public Competitor {

 public:
  void Build(const std::vector<KeyValue<KeyType>>& data) {
      data_ = data;
      
      std::vector<uint64_t> keys;
      for (auto itm : data) {
          keys.push_back(itm.key);
      }
      
      pgm_ = std::make_unique<PGMIndex<uint64_t, 128>>(keys);
  }

  uint64_t EqualityLookup(const KeyType lookup_key) const {
      auto approx_range = pgm_->find_approximate_position(lookup_key);
      auto lo = approx_range.lo;
      auto hi = approx_range.hi;

      size_t num_qualifying;
      return util::binary_search(data_, lookup_key, &num_qualifying,
                                 lo, hi);

  }

  std::string name() const {
    return "PGM";
  }

  std::size_t size() const {
      return pgm_->size_in_bytes() + data_.size() * sizeof(KeyValue<KeyType>);
  }

  bool applicable(bool unique, const std::string& data_filename) const {
      return true;
  }

 private:
    std::vector<KeyValue<KeyType>> data_;
    std::unique_ptr<PGMIndex<uint64_t, 128>> pgm_;
};

#endif //SOSDB_PGM_H
