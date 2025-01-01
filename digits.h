#ifndef __DIGITS__
#define __DIGITS__

#include <array>

#include "types.h"

using Factor = std::array<u8, 3>;
using Number = std::array<u8, 6>;

constexpr bool isPalindrome(Number n)
{
    return n[0] == n[5] && n[1] == n[4] && n[2] == n[3];
};

constexpr u16 toInt(const Factor& f)
{
    return 100 * f[0] + 10 * f[1] + f[2];
};

constexpr u16 multiply(const Factor& f1, const Factor& f2)
{
    return toInt(f1) * toInt(f2);
};

constexpr Factor toFactor(u16 n)
{   
    Factor f{0,0,0};
    Factor::size_type idx = 2;
    while (n > 0)
    {
        f[idx--] = static_cast<u8>(n % 10);
        n = static_cast<u16>(n / 10);
    }
    return f; 
};

constexpr Number toNumber(u32 n)
{
    Number nb{0,0,0,0,0,0};
    Number::size_type idx = 5;
    while (n > 0)
    {
        nb[idx--] = static_cast<u8>(n % 10);
        n = static_cast<u32>(n / 10);
    }
    return nb;         
};

#endif
