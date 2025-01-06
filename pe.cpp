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
#include <string_view>

#include <cassert>

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

template<int variant = 0>
/* constexpr */ 
u64 Problem8_LargestProductInASeries()
{
    constexpr std::string_view numString{"7316717653133062491922511967442657474235534919493496983520312774506326239578318016984801869478851843858615607891129494954595017379583319528532088055111254069874715852386305071569329096329522744304355766896648950445244523161731856403098711121722383113622298934233803081353362766142828064444866452387493035890729629049156044077239071381051585930796086670172427121883998797908792274921901699720888093776657273330010533678812202354218097512545405947522435258490771167055601360483958644670632441572215539753697817977846174064955149290862569321978468622482839722413756570560574902614079729686524145351004748216637048440319989000889524345065854122758866688116427171479924442928230863465674813919123162824586178664583591245665294765456828489128831426076900422421902267105562632111110937054421750694165896040807198403850962455444362981230987879927244284909188845801561660979191338754992005240636899125607176060588611646710940507754100225698315520005593572972571636269561882670428252483600823257530420752963450"};
    constexpr size_t windowWidth{ 13u };
   
    if (numString.size() < windowWidth)
        return 0ull;
    std::array<u64, numString.size()> digits;
    std::transform(begin(numString), end(numString), begin(digits), [](const char c) { return c - '0';});

    auto aggMult = [](u64 agg, int d) -> u64 { return agg * d;};
    u64 max{ std::accumulate(begin(digits), begin(digits) + windowWidth, 1ull, aggMult ) };
    std::string_view argMax{numString.substr(0u, windowWidth)};
    size_t maxPos{ 0u };
    u64 prod{ max };

    if constexpr (variant == 1)
    {
        size_t startIdx{ 0u };
        auto multiplier = [](auto&&... args) -> u64 { return (args * ...);  };
        for (std::tuple window: digits | std::views::adjacent<windowWidth>)
        {
            prod = std::apply(multiplier, window);
            if (prod > max)
            {
                max = prod;
                argMax = numString.substr(startIdx, windowWidth);
                maxPos = startIdx;
            }
            startIdx++;
        }
        std::print("Variant 'apply': {0} starting at {1} with sequence {2}\n", max, maxPos, argMax);
    }
    else if constexpr (variant == 0)
    {
        for(size_t startIdx = 1; startIdx + windowWidth < digits.size(); ++startIdx)
        {   
            prod = std::accumulate(begin(digits) + startIdx, begin(digits) + startIdx + windowWidth, 1ull, aggMult);
            if (prod > max)
            {
                max = prod;
                argMax = numString.substr(startIdx, windowWidth);
                maxPos = startIdx;
            }
        }
        std::print("Variant 'accumulate': {0} starting at {1} with sequence {2}\n", max, maxPos, argMax);
    }
    else
    {
        for(size_t startIdx = 1; startIdx + windowWidth < digits.size(); ++startIdx)
        {   
            prod = digits[startIdx];
            for (size_t idx = 1; idx < windowWidth; ++idx)
                prod *= digits[startIdx + idx];

            if (prod > max)
            {
                max = prod;
                argMax = numString.substr(startIdx, windowWidth);
                maxPos = startIdx;
            }
        }
        std::print("Variant 'classic': {0} starting at {1} with sequence {2}\n", max, maxPos, argMax);
        
    }
    return max;
}

u64 Problem9_SpecialPythagoreanTriplet()
{
    std::tuple<u32, u32, u32> solution;
    constexpr u32 sum{ 1'000u };
    for (u32 c = sum - 2u; c > 2u; --c)
    {
        const u32 ab = sum - c;
        for (u32 b = 1u; b < ab; ++b)
        {
            const u32 a = ab - b;
            const u32 c2 = a*a + b*b;
            if (c * c == c2)
            {
                solution = std::make_tuple(a, b, c);
                std::print("{0}**2 + {1}**2 = {2}**2, a*b*c={3}\n", a, b, c, a * b * c);
                return std::get<0>(solution) * std::get<1>(solution) * std::get<2>(solution);
            }        
        }
    }
    return 0u;
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
    // Problem7_10001stPrime
    // Problem8_LargestProductInASeries<2>

    Run(TestDummy, Problem9_SpecialPythagoreanTriplet);
    //TestDigits();
}