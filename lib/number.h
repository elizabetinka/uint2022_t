#pragma once
#include <cinttypes>
#include <iostream>


struct uint2022_t {
    static size_t const SIZE = 68;
    // длинное число храниться в массиве в обратном порядке.
    uint32_t mass[SIZE];
    // количество цифр в элементе mass
    static size_t const BUFFSIZE = 9;
    // основание системы счисления
    static size_t const MODUL = 1000000000;
    // количество ненулевых элементов в mass
    size_t count = 0;

    uint2022_t &operator=(const uint2022_t &copy) = default;
};

static_assert(sizeof(uint2022_t) <= 300, "Size of uint2022_t must be no higher than 300 bytes");

uint2022_t from_uint(uint32_t i);

uint2022_t from_string(const char* buff);

uint2022_t operator+(const uint2022_t& lhs, const uint2022_t& rhs);

uint2022_t operator-(const uint2022_t& lhs, const uint2022_t& rhs);

uint2022_t operator*(const uint2022_t& lhs, const uint2022_t& rhs);

uint2022_t operator/(const uint2022_t& lhs, const uint2022_t& rhs);

uint2022_t operator/(const uint2022_t& lhs, const uint32_t rhs);

bool operator==(const uint2022_t& lhs, const uint2022_t& rhs);

bool operator!=(const uint2022_t& lhs, const uint2022_t& rhs);

bool operator>=(const uint2022_t& lhs, const uint2022_t& rhs);

bool operator<(const uint2022_t& lhs, const uint2022_t& rhs);

std::ostream& operator<<(std::ostream& stream, const uint2022_t& value);
