#include <vector>
#include <array>
#include <algorithm>
#include <numeric>
#include <print>
#include <ranges>
#include <cstdint>
#include <chrono>
#include <cassert>
#include "windows.h"

using u64 = uint64_t;

u64 Problem1_MultiplesOf3Or5()
{
    auto numbers = std::views::iota(1, 1000)
    | std::views::filter([](int i) { return i % 3 == 0 || i % 5 == 0;});
    u64 sum = std::ranges::fold_left(numbers, 0, std::plus<int>());
    return sum;
}

constexpr u64 Problem2_EvenFibonacciNumbers()
{
    std::array<u64, 2> fibos{1, 2};
    u64 sum{2};
    u64 next{fibos[0] + fibos[1]};
    uint8_t idx{0};
    while(next <= 4'000'000ull)
    {
        fibos[idx] = next;
        sum += (next & 1) * next;  
        next = fibos[0] + fibos[1];
        idx = 1 - idx;
    } 
    return sum;
}

namespace {
    std::vector<u64> primesUpTo(u64 limit)
    {
        std::vector<u64> primes(limit - 1u);  // b/c we start with 2 we need one element less
        std::iota(begin(primes), end(primes), 2ull);
        auto it = primes.begin();
        auto start = it;
        auto divisible = [&it](u64 z) -> bool { return z % *it == 0;};
        while (it != primes.end())
        {
            start = std::next(it);
            primes.erase(std::remove_if(start, end(primes), divisible), end(primes));
            it++;
        }
        return primes;
    }
}

void Test()
{
    {
        auto list = primesUpTo(12ull);
        std::vector<u64> expected = {2,3,5,7,11};
        assert(list == expected);
    }
    {
        auto list = primesUpTo(7ull);
        std::vector<u64> expected = {2,3,5,7};
        assert(list == expected);
    }
}

u64 Problem3_LargestPrimeFactor()
{
    constexpr u64 number = 600'851'475'143ull;
    const u64 sqrtNumber = 1 + static_cast<u64>(trunc(sqrt(number)));

    std::vector<u64> factors = primesUpTo(sqrtNumber);
    for (;;)
    {

    }

}

int main(int argc, char** argv)
{
    SetConsoleOutputCP(CP_UTF8);

    Test();

    auto start = std::chrono::high_resolution_clock::now();
    // u64 result = Problem1_MultiplesOf3Or5();
    u64 result = Problem2_EvenFibonacciNumbers();
    auto end = std::chrono::high_resolution_clock::now();
    std::print("{0}\n", result);
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::print("Execution time: {0}µs.\n", duration.count());
}