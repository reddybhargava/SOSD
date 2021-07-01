#include <iostream>
#include <queue>
#include <random>
#include <vector>

#include "searches/branching_binary_search.h"
#include "util.h"

using namespace std;

// We ensure that there are no more qualifying entries for a given lookup
// than specified here.
// Setting a low value (e.g., 100) here ensures that the checksum computation
// doesn't dominate performance.
constexpr size_t max_num_qualifying = 100;

// Maximum number of retries to find a lookup key that has at most
// `max_num_qualifying entries`.
constexpr size_t max_num_retries = 100;

// //=========================================================================
// //= Multiplicative LCG for generating uniform(0.0, 1.0) random numbers    =
// //=   - x_n = 7^5*x_(n-1)mod(2^31 - 1)                                    =
// //=   - With x seeded to 1 the 10000th x value should be 1043618065       =
// //=   - From R. Jain, "The Art of Computer Systems Performance Analysis," =
// //=     John Wiley & Sons, 1991. (Page 443, Figure 26.2)                  =
// //=========================================================================
double rand_val(int seed) {
  const long a = 16807;       // Multiplier
  const long m = 2147483647;  // Modulus
  const long q = 127773;      // m div a
  const long r = 2836;        // m mod a
  static long x;              // Random int value
  long x_div_q;               // x divided by q
  long x_mod_q;               // x modulo q
  long x_new;                 // New x value

  // Set the seed if argument is non-zero and then return zero
  if (seed > 0) {
    x = seed;
    return (0.0);
  }

  // RNG using integer arithmetic
  x_div_q = x / q;
  x_mod_q = x % q;
  x_new = (a * x_mod_q) - (r * x_div_q);
  if (x_new > 0)
    x = x_new;
  else
    x = x_new + m;

  // Return a random value between 0.0 and 1.0
  return ((double)x / m);
}

//===========================================================================
//=  Function to generate Zipf (power law) distributed random variables     =
//=    - Input: alpha and N                                                 =
//=    - Output: Returns with Zipf distributed random variable              =
//===========================================================================

//----- Constants -----------------------------------------------------------
#define FALSE 0  // Boolean false
#define TRUE 1   // Boolean true

// int zipf(double alpha, int n) {

//   static int first = TRUE;      // Static first time flag
//   static double c = 0;          // Normalization constant
//   double z;                     // Uniform random number (0 < z < 1)
//   double sum_prob;              // Sum of probabilities
//   double zipf_value = 0;            // Computed exponential value to be
//   returned int    i;                     // Loop counter

//   // Compute normalization constant on first call only
//   if (first == TRUE) {
//     for (i=1; i<=n; i++)
//       c = c + (1.0 / pow((double) i, alpha));
//     c = 1.0 / c;
//     first = FALSE;
//   }

//   // Pull a uniform random number (0 < z < 1)
//   do {
//     z = rand_val(0);
//   } while ((z == 0) || (z == 1));

//   // Map z to the value
//   sum_prob = 0;
//   for (i=1; i<=n; i++) {
//     sum_prob = sum_prob + c / pow((double) i, alpha);
//     if (sum_prob >= z) {
//       zipf_value = i;
//       break;
//     }
//   }

//   // Assert that zipf_value is between 1 and N
//   assert((zipf_value >=1) && (zipf_value <= n));

//   return(zipf_value);
// }

int zipf(double alpha, int n) {
  static int first = TRUE;   // Static first time flag
  static double c = 0;       // Normalization constant
  static double* sum_probs;  // Pre-calculated sum of probabilities
  double z;                  // Uniform random number (0 < z < 1)
  int zipf_value;            // Computed exponential value to be returned
  int i;                     // Loop counter
  int low, high, mid;        // Binary-search bounds

  // Compute normalization constant on first call only
  if (first == TRUE) {
    for (i = 1; i <= n; i++) c = c + (1.0 / pow((double)i, alpha));
    c = 1.0 / c;

    sum_probs = (double*)malloc((n + 1) * sizeof(*sum_probs));
    sum_probs[0] = 0;
    for (i = 1; i <= n; i++) {
      sum_probs[i] = sum_probs[i - 1] + c / pow((double)i, alpha);
    }
    first = FALSE;
  }

  // Pull a uniform random number (0 < z < 1)
  do {
    z = rand_val(0);
  } while ((z == 0) || (z == 1));

  // Map z to the value
  low = 1, high = n, mid;
  do {
    mid = floor((low + high) / 2);
    if (sum_probs[mid] >= z && sum_probs[mid - 1] < z) {
      zipf_value = mid;
      break;
    } else if (sum_probs[mid] >= z) {
      high = mid - 1;
    } else {
      low = mid + 1;
    }
  } while (low <= high);

  // Assert that zipf_value is between 1 and N
  assert((zipf_value >= 1) && (zipf_value <= n));

  return (zipf_value);
}

template <class KeyType, class T>
vector<EqualityLookup<KeyType>> generate_equality_lookups_zipf(
    const vector<Row<KeyType>>& data, const vector<T>& unique_keys,
    const size_t num_lookups, const double alpha) {
  vector<EqualityLookup<KeyType>> lookups;
  lookups.reserve(num_lookups);
  BranchingBinarySearch<KeyType> bbs;
  const uint64_t max_value = unique_keys.size() - 1;

  // random seed value
  rand_val(alpha * 100);

  for (uint64_t i = 0; i < num_lookups; i++) {
    const int zipf_rv = zipf(alpha, max_value);
    const KeyType lookup_key = unique_keys[zipf_rv];

    size_t num_qualifying;
    const uint64_t result =
        bbs.search(data, lookup_key, &num_qualifying, 0, data.size());

    const EqualityLookup<KeyType> lookup_entry = {lookup_key, result};
    lookups.push_back(lookup_entry);
  }

  return lookups;
}

// Generates `num_lookups` lookups such that `negative_lookup_ratio` lookups are
// negative.
template <class KeyType, class T>
vector<EqualityLookup<KeyType>> generate_equality_lookups(
    const vector<Row<KeyType>>& data, const vector<T>& unique_keys,
    const size_t num_lookups, const double negative_lookup_ratio = 0.0,
    const uint64_t num_repetitions = 0, uint64_t min_num_repetititons_key = 0,
    uint64_t max_num_repetititons_key = 100, bool is_zipf = false,
    double zipf_alpha = 1.0) {
  if (is_zipf)
    return generate_equality_lookups_zipf(data, unique_keys, num_lookups,
                                          zipf_alpha);

  vector<EqualityLookup<KeyType>> lookups;
  lookups.reserve(num_lookups + num_repetitions);
  util::FastRandom ranny(42);
  size_t num_generated = 0;
  size_t num_retries = 0;
  BranchingBinarySearch<KeyType> bbs;
  queue<pair<EqualityLookup<KeyType>, uint64_t>>
      repetitions;  // Queue used to store the keys to repeat - pair{key,
                    // num_repetitions_of_that_key}
  uint64_t num_repetitions_left = num_repetitions;

  // std::cout << num_repetitions_left << " " << num_repetitions << " " <<
  // min_num_repetititons_key << " " << max_num_repetititons_key << std::endl;

  // Required to generate negative lookups within data domain.
  const KeyType min_key = data.front().key;
  const KeyType max_key = data.back().key;

  while (num_generated < num_lookups) {
    if (num_retries == 0 && negative_lookup_ratio > 0.0 &&
        ranny.ScaleFactor() < negative_lookup_ratio) {
      // Generate negative lookup.
      KeyType negative_lookup;
      size_t num_qualifying = 1;
      while (num_qualifying > 0) {
        // Draw lookup key from within data domain.
        negative_lookup = (ranny.ScaleFactor() * (max_key - min_key)) + min_key;
        bbs.search(data, negative_lookup, &num_qualifying, 0, data.size());
      }
      lookups.push_back({negative_lookup, util::NOT_FOUND});
      ++num_generated;
      continue;
    }

    // Generate positive lookup.

    // Draw lookup key from unique keys.
    const uint64_t offset = ranny.RandUint32(0, unique_keys.size() - 1);
    const KeyType lookup_key = unique_keys[offset];

    // Perform binary search on original keys.
    size_t num_qualifying;
    const uint64_t result =
        bbs.search(data, lookup_key, &num_qualifying, 0, data.size());

    if (num_qualifying > max_num_qualifying) {
      // Too many qualifying entries.
      ++num_retries;
      if (num_retries > max_num_retries)
        util::fail("generate_equality_lookups: exceeded max number of retries");
      // Try a different lookup key.
      continue;
    }
    const EqualityLookup<KeyType> lookup_entry = {lookup_key, result};
    lookups.push_back(lookup_entry);

    if (num_repetitions_left > 0) {
      // std::cout << "left: " << num_repetitions_left << " " << num_repetitions
      // << " " << min_num_repetititons_key << " " << max_num_repetititons_key
      // << std::endl;

      const uint64_t num_repetitions_key =
          ranny.RandUint32(min_num_repetititons_key, max_num_repetititons_key);
      if (num_repetitions_key > 0) {
        repetitions.push(make_pair(lookup_entry, num_repetitions_key));
      }

      if (!repetitions.empty()) {
        pair<EqualityLookup<KeyType>, uint64_t> repetition_entry =
            repetitions.front();
        repetitions.pop();
        if (repetition_entry.second > 1) {
          repetition_entry.second--;
          repetitions.push(repetition_entry);
        }
        lookups.push_back(repetition_entry.first);
      }

      num_repetitions_left -= num_repetitions_key;
      // To make sure sum(num_repetitions_key) doesn't exceed the
      // num_repetitions
      if (num_repetitions_left < max_num_repetititons_key) {
        max_num_repetititons_key = num_repetitions_left;
      }
      if (max_num_repetititons_key < min_num_repetititons_key) {
        min_num_repetititons_key = 0;
      }
    }

    ++num_generated;
    num_retries = 0;
  }
  // Push the remaining keys from the repetitions queue to lookups vector
  while (!repetitions.empty()) {
    pair<EqualityLookup<KeyType>, uint64_t> repetition_entry =
        repetitions.front();
    repetitions.pop();
    if (repetition_entry.second > 1) {
      repetition_entry.second--;
      repetitions.push(repetition_entry);
    }
    lookups.push_back(repetition_entry.first);
  }

  return lookups;
}

template <class KeyType>
vector<EqualityLookup<KeyType>> generate_equality_lookups_from_trace(
    const vector<Row<KeyType>>& data, const vector<KeyType> keys) {
  vector<EqualityLookup<KeyType>> lookups;
  BranchingBinarySearch<KeyType> bbs;

  size_t nq;
  for (KeyType key : keys) {
    lookups.push_back({key, bbs.search(data, key, &nq, 0, keys.size())});
  }

  return lookups;
}

const string to_nice_number(uint64_t num) {
  const uint64_t THOUSAND = 1000;
  const uint64_t MILLION = 1000 * THOUSAND;
  const uint64_t BILLION = 1000 * MILLION;

  if (num >= BILLION && (num / BILLION) * BILLION == num) {
    return to_string(num / BILLION) + "B";
  }
  if (num >= MILLION && (num / MILLION) * MILLION == num) {
    return to_string(num / MILLION) + "M";
  }
  if (num >= THOUSAND && (num / THOUSAND) * THOUSAND == num) {
    return to_string(num / THOUSAND) + "K";
  }
  return to_string(num);
}

template <class KeyType>
void print_equality_lookup_stats(
    const vector<EqualityLookup<KeyType>>& lookups) {
  size_t negative_count = 0;
  for (const auto& lookup : lookups)
    if (lookup.result == util::NOT_FOUND) ++negative_count;
  std::cout << "negative lookup ratio: "
            << static_cast<double>(negative_count) / lookups.size()
            << std::endl;
}

int main(int argc, char* argv[]) {
  if (argc < 3)
    util::fail(
        "usage: ./generate <data file> <num lookups> [negative lookup ratio]");

  const string filename = argv[1];
  const DataType type = util::resolve_type(filename);
  size_t num_lookups = stoull(argv[2]);
  double negative_lookup_ratio = 0.0;
  if (argc >= 4) negative_lookup_ratio = stod(argv[3]);
  if (negative_lookup_ratio < 0 || negative_lookup_ratio > 1) {
    util::fail("negative lookup ratio must be between 0 and 1.");
  }
  uint64_t num_repetitions = 0, min_num_repetititons_key = 0,
           max_num_repetititons_key = 0;
  bool is_zipf = false;
  if (argc >= 5) num_repetitions = stoull(argv[4]);
  if (argc >= 7)
    min_num_repetititons_key = stoull(argv[5]),
    max_num_repetititons_key = stoull(argv[6]);
  if (argc >= 8) {
    if (stoull(argv[7])) is_zipf = true;
  }
  double zipf_alpha = 1.0;
  if (argc >= 9) zipf_alpha = stod(argv[8]);

  // std::cout << num_repetitions << " " << min_num_repetititons_key << " " <<
  // max_num_repetititons_key << std::endl;

  switch (type) {
    case DataType::UINT32: {
      // Load data.
      const vector<uint32_t> keys = util::load_data<uint32_t>(filename);

      if (!is_sorted(keys.begin(), keys.end()))
        util::fail("keys have to be sorted (read 32-bit keys)");

      // Get duplicate-free copy: we draw lookup keys from unique keys.
      vector<uint32_t> unique_keys = util::remove_duplicates(keys);

      // Add artificial values to original keys.
      vector<Row<uint32_t>> data = util::add_values(keys);

      // Generate benchmarks.
      vector<EqualityLookup<uint32_t>> equality_lookups;
      if (filename.find("corp") != std::string::npos) {
        // load the precomputed trace
        std::cout << "generating lookups from trace" << std::endl;
        std::string trace_name = filename;
        trace_name.replace(trace_name.find("corp_200M_uint32"),
                           sizeof("corp_200M_uint32") - 1, "corp_trace_uint32");

        vector<uint32_t> keys = util::load_data<uint32_t>(trace_name);
        num_lookups = keys.size();
        equality_lookups = generate_equality_lookups_from_trace(data, keys);
      } else {
        equality_lookups = generate_equality_lookups(
            data, unique_keys, num_lookups, negative_lookup_ratio,
            num_repetitions, min_num_repetititons_key, max_num_repetititons_key,
            is_zipf, zipf_alpha);
      }

      print_equality_lookup_stats(equality_lookups);
      if (negative_lookup_ratio == 1) {
        util::write_data(equality_lookups,
                         filename + "_inserts_" + to_nice_number(num_lookups));
      } else if (num_repetitions > 0) {
        util::write_data(equality_lookups,
                         filename + "_equality_lookups_repetitions_" +
                             to_nice_number(num_lookups + num_repetitions));
      } else if (is_zipf) {
        util::write_data(equality_lookups,
                         filename + "_equality_lookups_zipf_" +
                             to_nice_number(num_lookups) + "_" +
                             std::to_string((int)zipf_alpha));
      } else {
        util::write_data(equality_lookups, filename + "_equality_lookups_" +
                                               to_nice_number(num_lookups));
      }

      break;
    }
    case DataType::UINT64: {
      // Load data.
      const vector<uint64_t> keys = util::load_data<uint64_t>(filename);

      if (!is_sorted(keys.begin(), keys.end()))
        util::fail("keys have to be sorted (read 64-bit keys)");

      // Get duplicate-free copy: we draw lookup keys from unique keys.
      vector<uint64_t> unique_keys = util::remove_duplicates(keys);

      // Add artificial values to original keys.
      vector<Row<uint64_t>> data = util::add_values(keys);

      // Generate benchmarks.
      vector<EqualityLookup<uint64_t>> equality_lookups;
      if (filename.find("corp") != std::string::npos) {
        // load the precomputed trace
        std::cout << "generating lookups from trace" << std::endl;
        std::string trace_name = filename;
        trace_name.replace(trace_name.find("corp_200M_uint64"),
                           sizeof("corp_200M_uint64") - 1, "corp_trace_uint64");

        vector<uint64_t> keys = util::load_data<uint64_t>(trace_name);
        num_lookups = keys.size();
        equality_lookups = generate_equality_lookups_from_trace(data, keys);
      } else {
        equality_lookups = generate_equality_lookups(
            data, unique_keys, num_lookups, negative_lookup_ratio,
            num_repetitions, min_num_repetititons_key, max_num_repetititons_key,
            is_zipf, zipf_alpha);
      }

      print_equality_lookup_stats(equality_lookups);
      if (negative_lookup_ratio == 1) {
        util::write_data(equality_lookups,
                         filename + "_inserts_" + to_nice_number(num_lookups));
      } else if (num_repetitions > 0) {
        std::cout << equality_lookups.size() << std::endl;
        util::write_data(equality_lookups,
                         filename + "_equality_lookups_repetitions_" +
                             to_nice_number(num_lookups + num_repetitions));
      } else if (is_zipf) {
        util::write_data(equality_lookups,
                         filename + "_equality_lookups_zipf_" +
                             to_nice_number(num_lookups) + "_" +
                             std::to_string((int)zipf_alpha));
      } else {
        util::write_data(equality_lookups, filename + "_equality_lookups_" +
                                               to_nice_number(num_lookups));
      }

      break;
    }
  }

  return 0;
}
