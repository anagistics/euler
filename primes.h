#ifndef __PRIMES__
#define __PRIMES__

#include <vector>
#include <set>
#include <algorithm>
#include <numeric>
#include <ranges>

#include "types.h"

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
    std::vector<u64> primesSoFar;
    primesSoFar.reserve(static_cast<size_t>(ceil(number / log(number))));
    primesSoFar.push_back(2);
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

std::vector<u64> primeFactors(u64 number)
{
    if (number <= 3)
        return { number };

    std::set<u64> primes{2ull};
    std::vector<u64> factors;
    while ((number & 1ull) == 0)
    {
        factors.push_back(2ull);
        number = number >> 1;
    }
    u64 i{3ull};
    auto divides = [&i](u64 p) -> bool { return i % p == 0; };
    u64 limit = static_cast<u64>(floor(sqrt(number)));
    while (i <= limit)
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
            limit = static_cast<u64>(floor(sqrt(number)));
        }
        i += 2;
    }
    if (number > 1)
        factors.push_back(number);
    std::sort(begin(factors), end(factors));
    return factors;
}

std::set<u64> divisors(u64 number)
{
    std::set<u64> divisors;
    auto pfactors = primeFactors(number);
    if (pfactors.size() == 0)
        return divisors;

    for (auto startIt = begin(pfactors); startIt != end(pfactors); ++startIt)
    {
        for (auto endIt = startIt; endIt != end(pfactors); ++endIt)
        {
            auto divisor = std::accumulate(startIt, std::next(endIt), 1ull, std::multiplies<u64>());
            divisors.insert(divisor);
        }
    }
    return divisors;
}


#endif