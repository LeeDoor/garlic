#include <algorithm>
#include <iostream>
#include <vector>
#include "benchmark.hpp"

int main() {
    constexpr size_t s = 1000000;
    std::vector<int> arr(s);
    application::Benchmark bm;
    auto init_rands = bm.measure([&] {
        for(size_t i = 0; i < s; ++i) arr[i] = rand();
   });
    auto sort_chaos = bm.measure([&] {
        std::sort(arr.begin(), arr.end());
    });
    auto sort_sorted = bm.measure([&] {
        std::sort(arr.begin(), arr.end());
    });
    std::cout 
        << "init random values: " << init_rands << std::endl
        << "sort it: " << sort_chaos << std::endl
        << "sort again: " << sort_sorted << std::endl;
}
