#ifndef __TEST_DIGITS__
#define __TEST_DIGITS__

#include <cassert>
#include <array>
#include <print>

#include "digits.h"
#include "types.h"

void TestDigits()
{
    std::print("Starting digits tests...");
    {
        Factor f1 = {1,2,3};
        u16 i = toInt(f1);
        assert(i == 123);
    }
    {
        u16 i = 253;
        Factor f1 = toFactor(i);
        assert(f1[0] == 2 && f1[1] == 5 && f1[2] == 3);
    }
    {
        u32 i = 123456;
        Number n = toNumber(i);
        assert(n[0] == 1 && n[1] == 2 && n[2] == 3 && n[3] == 4 && n[4] == 5 && n[5] == 6);
    }
    std::print("done.\n");
}

#endif