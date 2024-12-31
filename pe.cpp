#include <vector>
#include <set>
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

    std::vector<u64> primesUpTo2(u64 number)
    {
        std::vector<u64> primesSoFar = {2};
        u64 i{3};
        auto divides = [&i](u64 p) -> bool { return i % p == 0;};
        while (i <= number)
        {
            auto it = std::find_if(begin(primesSoFar), end(primesSoFar), divides);
            if (it == end(primesSoFar))
                primesSoFar.push_back(i);
            i+=2;
        }
        return primesSoFar;
    }

    std::vector<u64> primeFactors0(u64 number)
    {
        std::vector<u64> factors;
        std::vector<u64> primes{primesUpTo(number)};
        auto p = begin(primes);
        while (number > 1ull && p != end(primes))
        {
            if (number % *p == 0)
            {
                factors.push_back(*p);
                number = number / *p;
            }
            else
                p++;
        }
        return factors;
    }

    std::vector<u64> primeFactors(u64 number)
    {
        if (number <= 3)
        {
            if (number <= 1)
                return {};
            return {number};
        }

        std::set<u64> primes{2ull};
        std::vector<u64> factors;
        while ((number & 1ull) == 0)
        {
            factors.push_back(2ull);
            number = number >> 1;
        }
        u64 i{3ull};
        auto divides = [&i](u64 p) -> bool { return i % p == 0; };
        while (i <= number)
        {
            auto it = std::find_if(begin(primes), end(primes), divides);
            if (it == end(primes))
            {
                (void)primes.insert(i);
                while (number % i == 0)
                {
                    factors.push_back(i);
                    number = number / i;
                }
            }
            i += 2;
        }
        return factors;
    }
}

void Test()
{
    std::print("Starting tests...");
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
    {
        std::vector<u64> pf = primeFactors(2);
        std::vector<u64> expected = {2};
        assert(pf == expected);
    }
    {
        std::vector<u64> pf = primeFactors(4);
        std::vector<u64> expected = {2, 2};
        assert(pf == expected);
    }
    {
        std::vector<u64> pf = primeFactors(6);
        std::vector<u64> expected = {2, 3};
        assert(pf == expected);
    }
    {
        std::vector<u64> pf = primeFactors(31);
        std::vector<u64> expected = {31};
        assert(pf == expected);
    }
    {
        std::vector<u64> pf = primeFactors(35);
        std::vector<u64> expected = {5, 7};
        assert(pf == expected);
    }
    {
        std::vector<u64> pf = primeFactors(143);
        std::vector<u64> expected = {11, 13};
        assert(pf == expected);
    }
    {
        std::vector<u64> pf = primeFactors(2 * 61);
        std::vector<u64> expected = {2, 61};
        assert(pf == expected);
    }

    std::print("done.\n");
}

u64 Problem3_LargestPrimeFactor()
{
    constexpr u64 number = 600'851'475'143ull;
    std::vector<u64> factors = primeFactors(number);
    auto it = std::max_element(begin(factors), end(factors));
    return *it;
}

int main(int, char**)
{
    SetConsoleOutputCP(CP_UTF8);

    Test();

    auto start = std::chrono::high_resolution_clock::now();
    // u64 result = Problem1_MultiplesOf3Or5();
    // u64 result = Problem2_EvenFibonacciNumbers();
    u64 result = Problem3_LargestPrimeFactor();
    auto end = std::chrono::high_resolution_clock::now();
    std::print("{0}\n", result);
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::print("Execution time: {0}µs.\n", duration.count());
}