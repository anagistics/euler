#ifndef __TEST_PRIMES__
#define __TEST_PRIMES__

#include <print>
#include <cassert>
#include <vector>

#include "types.h"
#include "primes.h"

void TestPrimes()
{
    std::print("Starting prime tests...");
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

#endif
