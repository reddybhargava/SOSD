#include <iostream>
// #include <string>
#include "util.h"
#include "searches/branching_binary_search.h"

using namespace std;

template<class KeyType>
vector<KeyType> getKeys(string dataset, DataType type) {
    
    // return keys;
}


int main() {

    vector<string> datasets = {
        "books_200M_uint32",
        "books_200M_uint64",
        "fb_200M_uint64",
        "osm_cellids_200M_uint64",
        "wiki_ts_200M_uint64"
    };

    vector<string> lookupSizes = { "1K", "10K", "50K", "100K", "500K", "1M" };
    vector<string> alphaValues = { "1", "2", "3"};

    string filename;
    for (string dataset : datasets) {
        const DataType type = util::resolve_type(dataset);
        switch (type) {
            case DataType::UINT32: {
                const vector<uint32_t> keys = util::load_data<uint32_t>(dataset);
                break ;
            }
            case DataType::UINT64: {
                const vector<uint64_t> keys = util::load_data<uint64_t>(dataset);
                break ;
            }
        }

        for(string lookupSize : lookupSizes) {
            for(string alpha: alphaValues) {
                filename = "data/" + dataset + "_equality_lookups_zipf_" + lookupSize + "_" + alpha + "_python";
            
                std::ifstream file(filename);
                std::string str; 
                while (std::getline(file, str)) {
                // process string ...
                }
            }
        }
    }

    return 0;
}