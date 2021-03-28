#include <list>
#include <unordered_map>

class LRUCache {
    std::list<std::pair<uint64_t, uint64_t>> queue;
 
    std::unordered_map<uint64_t, std::list<std::pair<uint64_t, uint64_t>>::iterator> hash_map;
    uint64_t size;
 
public:
    LRUCache() = default;

    LRUCache(uint64_t n)
    {
        size = n;
    }

    std::pair<uint64_t, bool> lookup(uint64_t key)
    {
        bool found = true;
        uint64_t pos = 0;

        if (hash_map.find(key) == hash_map.end()) {
            found = false;
            if (queue.size() == size) {
                uint64_t last = queue.back().first;
                queue.pop_back();
    
                hash_map.erase(last);
            }
        }
    
        else
        {
            pos = hash_map[key]->second;
            queue.erase(hash_map[key]);
        }
    
        queue.push_front(std::make_pair(key, pos));
        hash_map[key] = queue.begin();
        return std::make_pair(pos, found);
    }

    void add_pos(uint64_t pos)
    {
        queue.begin()->second = pos;
    }
};
