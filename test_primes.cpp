#ifndef __TEST_PRIMES__
#define __TEST_PRIMES__

#include <print>
#include <cassert>
#include <vector>

#include "types.h"
#include "primes.h"

void TestPrimes()
{
    auto primeFunc = primesUpTo;
    std::print("Starting prime tests...");
    {
        std::vector<u64> pf = primeFunc(15);
        std::vector<u64> expected = {2, 3, 5, 7, 11, 13};
        assert(pf == expected);
    }
    {
        auto list = primeFunc(12ull);
        std::vector<u64> expected = {2,3,5,7,11};
        assert(list == expected);
    }
    {
        auto list = primeFunc(7ull);
        std::vector<u64> expected = {2,3,5,7};
        assert(list == expected);
    }
    {
        std::vector<u64> pf = primeFunc(2);
        std::vector<u64> expected = {2};
        assert(pf == expected);
    }
    {
        std::vector<u64> pf = primeFunc(4);
        std::vector<u64> expected = {2, 3};
        assert(pf == expected);
    }
    // test of prime factors
    primeFunc = primeFactors;
    {
        std::vector<u64> pf = primeFunc(4);
        std::vector<u64> expected = {2, 2};
        assert(pf == expected);
    }
    {
        std::vector<u64> pf = primeFunc(31);
        std::vector<u64> expected = {31};
        assert(pf == expected);
    }
    {
        std::vector<u64> pf = primeFunc(35);
        std::vector<u64> expected = {5, 7};
        assert(pf == expected);
    }
    {
        std::vector<u64> pf = primeFunc(143);
        std::vector<u64> expected = {11, 13};
        assert(pf == expected);
    }
    {
        std::vector<u64> pf = primeFunc(2 * 61);
        std::vector<u64> expected = {2, 61};
        assert(pf == expected);
    }
    {
        std::vector<u64> pf = primeFunc(0);
        std::vector<u64> expected = {0};
        assert(pf == expected);
    }
    {
        std::vector<u64> pf = primeFunc(1);
        std::vector<u64> expected = {1};
        assert(pf == expected);
    }
    {
        std::set<u64> dvs = divisors(6);
        std::set<u64> expected{2, 3, 6};
        assert(dvs == expected);
    }
    {
        std::set<u64> dvs = divisors(7);
        std::set<u64> expected{7};
        assert(dvs == expected);
    }
    {
        std::set<u64> dvs = divisors(1);
        std::set<u64> expected{1};
        assert(dvs == expected);
    }

    std::print("done.\n");
}

#endif
