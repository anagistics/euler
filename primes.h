#ifndef __PRIMES__
#define __PRIMES__

#include <vector>
#include <list>
#include <set>
#include <algorithm>
#include <numeric>
#include <ranges>
#include <bitset>

#include "types.h"

bool checkPrime(u64 cand, const std::vector<u64>& knownPrimes)
{
    const u64 lim = static_cast<u64>(ceil(sqrt(cand)));
    auto testPrimeIt = begin(knownPrimes);
    for (; testPrimeIt != end(knownPrimes) && *testPrimeIt <= lim; ++testPrimeIt)
    {
        if (cand % *testPrimeIt == 0)
            return false;
    }
    return true;
}

std::vector<u64> primesUpTo(u64 limit)
{
    constexpr u64 tableSize { 2'000'000ull };
    std::vector<u64> primesFound;
    primesFound.reserve(static_cast<size_t>(ceil(limit / log(limit))));
    primesFound.push_back(2ull);
    std::bitset<tableSize> skipList; // all numbers incl the even ones. By default false
    for (size_t z: {0u, 1u})
        skipList[z] = true;
    for (size_t z = 4u; z <= limit; z += 2)
        skipList[z] = true;
    u64 cand = 3ull;
    while (cand <= limit)
    {
        while (skipList[cand] && cand <= limit)
          cand++;
        if (cand > limit)
            break;
        bool isPrime = checkPrime(cand, primesFound);
        if (isPrime)
        {
            primesFound.push_back(cand);
            for (size_t z = 2 * cand; z <= limit; z += cand)
              skipList[z] = true;
        }
        cand++;
    }
    return primesFound;
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