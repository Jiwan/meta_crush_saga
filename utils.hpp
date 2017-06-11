//
// Created by jguegant on 2017-06-08.
//

#ifndef TEMPLATE_CRUSH_SAGA_UTILS_HPP
#define TEMPLATE_CRUSH_SAGA_UTILS_HPP

#include <utility>

#define CONSTEXPR constexpr

constexpr int abs(int x)
{
    int const mask = x >> sizeof(int) * 8 - 1;

    return (x + mask) ^ mask;
}

template <class T>
constexpr void swap(T& t1, T& t2)
{
    T tmp = t1;
    t1 = t2;
    t2 = tmp;
}

#endif //TEMPLATE_CRUSH_SAGA_UTILS_HPP
