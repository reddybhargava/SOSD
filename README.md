```
   _____ ____  _____ ____ 
  / ___// __ \/ ___// __ \
  \__ \/ / / /\__ \/ / / /
 ___/ / /_/ /___/ / /_/ / 
/____/\____//____/_____/  
                          
```

Search on Sorted Data Benchmark
====

[![Build Status](https://drone.rm.cab/api/badges/learnedsystems/SOSD/status.svg)](https://drone.rm.cab/learnedsystems/SOSD)

[SOSD](https://learned.systems/papers/sosd.pdf) is a benchmark to compare (learned) index structures on equality lookup performance over densely packed, sorted data.
It comes with state-of-the-art baseline implementations to compare against and many datasets to compare on.
Each dataset consists of 200 million to 800 million 32-bit or 64-bit unsigned integers. 

## Usage instructions

We provide a number of scripts to automate things. Each is located in the `scripts` directory, but should be executed from the repository root.

## Running the benchmark

* `scripts/setup_anywhere.sh` downloads the required tools and packages to run the benchmark
* `scripts/download.sh` downloads and stores required data from the Internet
* `scripts/build_rmis.sh` compiles and builds the RMIs for each dataset
  * `scripts/download_rmis.sh` will download pre-built RMIs instead, which may be faster. You'll need to run `build_rmis.sh` if you want to measure build times on your platform.
* `scripts/prepare.sh` constructs query workloads and compiles the benchmark 
* `scripts/execute_lookups_zipf.sh [cold|perf]` executes the benchmark on each zipfian workload, storing the results in `results`
* `scripts/execute_lookups_repetitions.sh [cold|perf]` executes the benchmark on each repetitions workload, storing the results in `results`

Build times can be long, as we make aggressive use of templates to ensure we do not accidentally measure vtable lookup time. For development, this can be annoying: you can set `USE_FAST_MODE` in `config.h` to disable some features and get a faster build time.

## Cite

If you use this benchmark in your own work, please cite our paper:

```
@article{sosd,
  title={SOSD: A Benchmark for Learned Indexes},
  author={Kipf, Andreas and Marcus, Ryan and van Renen, Alexander and Stoian, Mihail and Kemper, Alfons and Kraska, Tim and Neumann, Thomas},
  journal={NeurIPS Workshop on Machine Learning for Systems},
  year={2019}
}
```
