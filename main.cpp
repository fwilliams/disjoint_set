#include <iostream>
#include <random>
#include <chrono>
#include <cassert>

#include "./disjoint_set.h"

using namespace std;
using namespace std::chrono;


struct benchmark_stats {
    size_t num_unions = 0;
    size_t num_finds = 0;
    size_t num_elements = 0;
    size_t num_failed_unions = 0;
    size_t insert_time_us = 0;
    size_t union_time_us = 0;
    size_t find_time_us = 0;
    size_t num_sets = 0;

    void Print() {
        cout << "Num elements:  " << num_elements << endl <<
                "Num unions:    " << num_unions << endl <<
                "Num finds:     " << num_finds << endl <<
                "Insert time:   " << insert_time_us << " us (" << (float)insert_time_us/num_elements << " us/insert)" << endl <<
                "Union time:    " << union_time_us << " us (" << (float)union_time_us/num_unions << " us/union)" << endl <<
                "Find time:     " << find_time_us << " us (" << (float)find_time_us/num_finds << " us/find)" << endl <<
                "Failed union:  " << num_failed_unions << endl <<
                "Num sets:      " << num_sets << endl;
    }
};

benchmark_stats run_benchmark(size_t num_elements, size_t num_unions, size_t num_finds) {
    benchmark_stats stats;
    stats.num_elements = num_elements;
    stats.num_unions = num_unions;
    stats.num_finds = num_finds;

    std::mt19937 rng;
    rng.seed(time(0));
    std::uniform_int_distribution<int> uniform_distribution(0, num_unions-1);

    DisjointSet<int> djs;

    cout << "Inserting elements..." << endl;
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    for (int i = 0; i < num_elements; i++) {
        djs.MakeSet(i);
    }
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    stats.insert_time_us = duration_cast<microseconds>(t2 - t1).count();

    cout << "Performing unions..." << endl;
    t1 = high_resolution_clock::now();
    for (int i = 0; i < num_unions; i++) {
        int i1 = uniform_distribution(rng);
        int i2 = uniform_distribution(rng);
        stats.num_failed_unions += djs.Union(i1, i2) ? 0 : 1;
    }
    t2 = high_resolution_clock::now();
    stats.union_time_us = duration_cast<microseconds>(t2 - t1).count();


    t1 = high_resolution_clock::now();
    for (int i = 0; i < num_finds; i++) {
        int value = uniform_distribution(rng);
        int res = djs.Find(value);

    }
    t2 = high_resolution_clock::now();
    stats.find_time_us = duration_cast<microseconds>(t2 - t1).count();

    stats.num_sets = djs.SetCount();

    return stats;
}

int main() {
    const size_t NUM_ELEMENTS = 100000000;
    const size_t NUM_UNIONS = 100000;
    const size_t NUM_FINDS = 200*NUM_UNIONS;

    benchmark_stats stats = run_benchmark(NUM_ELEMENTS, NUM_UNIONS, NUM_FINDS);
    stats.Print();
}
