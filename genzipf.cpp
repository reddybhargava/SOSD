#include <iostream>
#include "util.h"
#include "searches/branching_binary_search.h"

using namespace std;

template<class KeyType>
vector<EqualityLookup<KeyType>> getLookups(vector<KeyType> keys, string lookupFile) {
    BranchingBinarySearch<KeyType> bbs;
    vector<EqualityLookup<KeyType>> lookups;
    EqualityLookup<KeyType> lookup_entry;
    string str;
    size_t num_qualifying;

    vector<Row<KeyType>> data = util::add_values(keys);
    ifstream file(lookupFile);
    while (getline(file, str)) {
        const KeyType lookup_key = stoull(str);
        const uint64_t result = bbs.search(data, lookup_key, &num_qualifying, 0, data.size());
        
        lookup_entry = {lookup_key, result};
        lookups.push_back(lookup_entry);
    }

    return lookups;
}

template<typename KeyType>
void generateLookups(DataType type, string dataset) {
    vector<uint32_t> keys32;
    vector<uint64_t> keys64;
    string readLookupFile, writeLookupFile;
    vector<string> lookupSizes = { "1K", "10K", "50K", "100K", "500K", "1M"};
    vector<string> alphaValues = { "1", "2", "3"};
    bool first = true;
    const string datasetFile = "data/" + dataset;
    
    for(string lookupSize : lookupSizes) {
        for(string alpha: alphaValues) {
            readLookupFile = writeLookupFile = dataset + "_equality_lookups_zipf_" + lookupSize + "_" + alpha;
            readLookupFile = "data/zipf/" + readLookupFile + "_python";
            writeLookupFile = "data/" + writeLookupFile;
            cout << "reading " << readLookupFile << endl;
            switch (type) {
                case DataType::UINT32: {
                    if (first) {
                        keys32 = util::load_data<uint32_t>(datasetFile);
                        first = false;
                    }
                    vector<EqualityLookup<uint32_t>> 
                        lookups = getLookups<uint32_t>(keys32, readLookupFile);
                    util::write_data(lookups, writeLookupFile);
                    break ;
                }
                case DataType::UINT64: {
                    if (first) {
                        keys64 = util::load_data<uint64_t>(datasetFile);
                        first = false;
                    }vector<EqualityLookup<uint64_t>> 
                        lookups = getLookups<uint64_t>(keys64, readLookupFile);
                    util::write_data(lookups, writeLookupFile);
                    break ;
                }
            } 
        }
    }

}


int main() {

    vector<string> datasets = {
        "books_200M_uint32",
        "books_200M_uint64",
        "fb_200M_uint64",
        "osm_cellids_200M_uint64",
        "wiki_ts_200M_uint64"
    };

    
    for (string dataset : datasets) {
        const DataType type = util::resolve_type(dataset);
        switch (type) {
            case DataType::UINT32: {
                generateLookups<uint32_t>(type, dataset);
                break ;
            }
            case DataType::UINT64: {
                generateLookups<uint64_t>(type, dataset);
                break ;
            }
        }
    }

    return 0;
}