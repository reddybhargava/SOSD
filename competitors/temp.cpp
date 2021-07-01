#include <vector>
#include <iostream>

#include "pgm_index_cache_l.h"

int main()
{
  std::vector<uint64_t> keys = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  auto pgm_ = new PGMCacheL<uint64_t, 32>();
  pgm_.Build(keys);

  for (uint64_t key = 1; key <= 20; key++)
  {
    uint64_t temp = key > 10 ? key - 10 : key;

    auto range = pgm_.EqualityLookup(key);
    auto start = keys.begin() + range.lo;
    auto end = keys.begin() + range.hi;

    auto pos = std::lower_bound(start, end, key) - keys.begin();
    pgm_.WriteCache(key, pos);
    
    std::cout << pos << std::endl;
  }
  
  return 0;
}
