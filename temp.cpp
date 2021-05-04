#include <fstream>
#include <iterator>
#include <string>
#include <vector>
#include "util.h"

using namespace std;

int main(int argc, char* argv[]) {
    string filename = argv[1];
    vector<string> files(3);

    for(int i=0; i<3; i++) {
        // files[i] = "data/" + filename + "_200M_uint64_equality_lookups_zipf_1K_" + to_string(i+1);
        files[0] = "data/books_200M_uint64_equality_lookups_repetitions_40K";
        files[1] = "data/books_200M_uint64_equality_lookups_repetitions_60K";
        files[2] = "data/books_200M_uint64_equality_lookups_repetitions_75K";
    }

    vector<uint64_t> keys;  
    for(int i=0; i<3; i++) {
        keys = util::load_data<uint64_t>(files[i]);
        std::ofstream output_file(files[i]+"_temp");
        // std::ostream_iterator<std::string> output_iterator(output_file, "\n");
        // std::copy(keys.begin(), keys.end(), output_iterator);
        // std::ofstream outFile("my_file.txt");
        // the important part
        for (const auto &e : keys) 
            output_file << e << "\n";
    }

    return 0;
}


#if 0

#include <iostream>
#include "util.h"

using namespace std;

int main(int argc, char* argv[]) {
    string filename = argv[1];
    string filename1 = filename + "_200M_uint64_equality_lookups_zipf_1K_1";
    string filename2 = filename + "_200M_uint64_equality_lookups_zipf_1K_2";
    string filename3 = filename + "_200M_uint64_equality_lookups_zipf_1K_3";

    vector<uint64_t> keys;
    unordered_map<uint64_t, uint64_t> counts;
    
    keys = util::load_data<uint64_t>(filename1);
    for (auto i : keys) {
        counts[i]++;
    }

    cout << "{" ;
    for (auto i : counts) {
        cout << ", " << i.first << ":" << i.second;
    }
    cout << "}" << endl;

    keys = util::load_data<uint64_t>(filename2);
    counts.clear();
    for (auto i : keys) {
        counts[i]++;
    }

    cout << "{" ;
    for (auto i : counts) {
        cout << ", " << i.first << ":" << i.second;
    }
    cout << "}" << endl;

    keys = util::load_data<uint64_t>(filename3);
    counts.clear();
    for (auto i : keys) {
        counts[i]++;
    }

    cout << "{" ;
    for (auto i : counts) {
        cout << ", " << i.first << ":" << i.second;
    }
    cout << "}" << endl;
}

#endif
