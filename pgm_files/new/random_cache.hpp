#include <unordered_map>

class RandomCache {
  std::unordered_map<uint64_t, uint64_t> hash_map;
  size_t size;
  size_t curr_size;

public:
  RandomCache() = default;

  RandomCache(size_t n)
    : size(n), curr_size(0)
  {}

  std::pair<uint64_t, bool> lookup(uint64_t key)
  {
    bool found = true;
    auto pos = 0;

    if(hash_map.find(key) == hash_map.end())
    {
      if(curr_size == size)
      {
	hash_map.erase(hash_map.begin());
      }
      hash_map[key] = 0;

      found = false;
    }

    else
    {
      pos = hash_map[key];
    }

    return std::make_pair(pos, found);
  }

  void add_pos(uint64_t key, uint64_t pos)
  {
    hash_map[key] = pos;
  }
};
