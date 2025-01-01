#include <vector>
#include <array>
#include <algorithm>
#include <numeric>
#include <print>
#include <ranges>
#include <chrono>

#include "windows.h"

#include "types.h"
#include "primes.h"

u64 Problem1_MultiplesOf3Or5()
{
    auto numbers = std::views::iota(1ull, 1000ull)
    | std::views::filter([](int i) { return i % 3ull == 0 || i % 5ull == 0;});
    u64 sum = std::ranges::fold_left(numbers, 0ull, std::plus<int>());
    return sum;
}

constexpr u64 Problem2_EvenFibonacciNumbers()
{
    std::array<u64, 2> fibos{1ull, 2ull};
    u64 sum{2ull};
    u64 next{fibos[0] + fibos[1]};
    uint8_t idx{0};
    while(next <= 4'000'000ull)
    {
        fibos[idx] = next;
        sum += (next & 1ull) * next;  
        next = fibos[0] + fibos[1];
        idx = 1ull - idx;
    } 
    return sum;
}

u64 Problem3_LargestPrimeFactor()
{
    constexpr u64 number = 600'851'475'143ull;
    std::vector<u64> factors = primeFactors(number);
    auto it = std::max_element(begin(factors), end(factors));
    return *it;
}

// include test files
#include "test_primes.cpp"

int main(int, char**)
{
    SetConsoleOutputCP(CP_UTF8);

    TestPrimes();

    auto start = std::chrono::high_resolution_clock::now();
    // u64 result = Problem1_MultiplesOf3Or5();
    // u64 result = Problem2_EvenFibonacciNumbers();
    u64 result = Problem3_LargestPrimeFactor();
    auto end = std::chrono::high_resolution_clock::now();
    std::print("{0}\n", result);
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::print("Execution time: {0}µs.\n", duration.count());
}