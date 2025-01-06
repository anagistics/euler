#include <vector>
#include <array>
#include <map>
#include <set>
#include <algorithm>
#include <numeric>
#include <print>
#include <ranges>
#include <chrono>
#include <functional>
#include <utility>
#include <optional>
#include <concepts>

#include "windows.h"

#include "types.h"
#include "primes.h"
#include "digits.h"

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

constexpr u64 Problem4_LargestPalindromeProduct()
{
    u32 maxPalindrom{0};
    u16 maxN1{0};
    u16 maxN2{0};
    for (u16 n1 = 999; n1 >= 100; --n1)
    {
        for (u16 delta = 0; delta <= n1 - 100; ++delta)
        {
            u16 n2 = n1 - delta;
            u32 prod = n1 * n2;
            Number n = toNumber(prod);
            if (isPalindrome(n) && prod > maxPalindrom)
            {
                maxPalindrom = prod;
                maxN1 = n1;
                maxN2 = n2;
            }
        }
    }
    // if not constexpr: std::print("Max palindrom {0}={1} x {2}\n", maxPalindrom, maxN1, maxN2);
    return maxPalindrom;
}

u64 Problem5_SmallestMultiple()
{
    std::map<u64, size_t> factors;
    for (u8 i = 1; i <= 20; ++i)
    {
        auto pf = primeFactors(i);
        std::multiset<u64> pfFreq;
        std::copy(begin(pf), end(pf), std::inserter(pfFreq, end(pfFreq)));
        for (u64 elem: pfFreq)
        {
            if (factors.find(elem) == end(factors))
                factors[elem] = 1;
            factors[elem] = std::max<size_t>(factors[elem], pfFreq.count(elem));
        }
    }
    auto multiMultiplier = [](u64 cum, auto elem) -> u64
    {
        return cum * pow(elem.first, elem.second);
    };

    auto prod = std::accumulate(begin(factors), end(factors), 1, multiMultiplier);
    return prod;
}

constexpr u64 Problem6_SumSquareDifference_BruteForce()
{
    constexpr u8 limit = 100;
    constexpr u64 sum = limit * (limit + 1) / 2;
    constexpr u64 sumSqr = sum * sum;

    u64 soS = 0;
    for (auto idx : std::ranges::iota_view(1u, limit + 1u))
      soS += idx * idx;
    
    return sumSqr - soS;
}

template<typename ArgumentType, typename ResultType, typename GradientType>
requires std::integral<ArgumentType> && std::integral<GradientType>
std::optional<ResultType> BinarySearch(ArgumentType initialLow, ArgumentType initialHigh
, std::function<ResultType(ArgumentType)> operation
, std::function<GradientType(ResultType)> predicate)
{
    ArgumentType low{initialLow};
    ArgumentType high{initialHigh};
    
    while (low < high)
    {
        ArgumentType midpoint = (low + high) / 2;
        ResultType result = operation(midpoint);
        GradientType diff = predicate(result);
        if (diff == 0)
            return std::optional<ResultType>(result);
        if (diff > 0)
            high = midpoint + 1;
        else
            low = midpoint - 1;
    }
    return std::nullopt;
}

u64 Problem7_10001stPrime()
{
    u64 low{1'000}, high{200'000};
    u64 target = 10'001;

    using SizePrime = std::pair<size_t, u64>;
    auto operation = [](u64 limit) -> SizePrime { auto l = primesUpTo(limit); return std::make_pair<size_t, u64&>(l.size(), l.back()); };
    auto gradient = [target](SizePrime s) -> i64 { return s.first - target; };
    
    auto result = BinarySearch<u64, SizePrime, i64>(low, high, operation, gradient);
    return result.value().second;
}

// include test files
#include "test_primes.cpp"
#include "test_digits.cpp"

void Run(std::function<void()> tester, std::function<u64()> problem)
{
    tester();
    auto start = std::chrono::high_resolution_clock::now();
    u64 result = problem();
    auto end = std::chrono::high_resolution_clock::now();
    std::print("{0}\n", result);
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::print("Execution time: {0}µs.\n", duration.count());
}

void TestDummy()
{
    std::print("no tests.\n");
}

u64 ProblemDummy()
{
    std::print("no problem.\n");
    return 0;
}

int main(int, char**)
{
    SetConsoleOutputCP(CP_UTF8);

    // Problem1_MultiplesOf3Or5();
    // Problem2_EvenFibonacciNumbers();
    // Problem3_LargestPrimeFactor();
    // Problem4_LargestPalindromeProduct();
    // Problem5_SmallestMultiple;
    // Problem6_SumSquareDifference_BruteForce

    Run(TestPrimes, Problem7_10001stPrime);
    //TestDigits();
}