//
// Created by jguegant on 2017-06-08.
//

#ifndef META_CRUSH_SAGA_UTILS_HPP
#define META_CRUSH_SAGA_UTILS_HPP

#include <iterator>
#include <utility>

#define CONSTEXPR constexpr

constexpr int abs(int x)
{
    int const mask = x >> (sizeof(int) * 8 - 1);

    return (x + mask) ^ mask;
}

template <class T>
constexpr void swap(T& t1, T& t2)
{
    T tmp = t1;
    t1 = t2;
    t2 = tmp;
}

template <class InputIt, class T>
constexpr typename std::iterator_traits<InputIt>::difference_type
count(InputIt first, InputIt last, const T& value)
{
    typename std::iterator_traits<InputIt>::difference_type ret = 0;
    for (; first < last; ++first) {
        if (*first == value) {
            ret++;
        }
    }

    return ret;
}

template <class InputIt, class T>
constexpr InputIt find(InputIt first, InputIt last, const T& value)
{
    for (; first < last; ++first) {
        if (*first == value) {
            return first;
        }
    }

    return last;
}

constexpr auto digits_amount(int x)
{
    x = abs(x);

    return (x < 10 ? 1 : (x < 100 ? 2 : (x < 1000 ? 3 : (x < 10000 ? 4 : (x < 100000 ? 5 : (x < 1000000 ? 6 : (x < 10000000 ? 7 : (x < 100000000 ? 8 : (x < 1000000000 ? 9 : 10)))))))));
}

#endif  //TEMPLATE_CRUSH_SAGA_UTILS_HPP
