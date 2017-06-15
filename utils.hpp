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

template<class InputIt, class T>
constexpr typename std::iterator_traits<InputIt>::difference_type
count(InputIt first, InputIt last, const T& value)
{
    typename std::iterator_traits<InputIt>::difference_type ret = 0;
    for (; first != last; ++first) {
        if (*first == value) {
            ret++;
        }
    }

    return ret;
}

template<class InputIt, class UnaryPredicate>
constexpr InputIt find_if(InputIt first, InputIt last, UnaryPredicate p)
{
    for (; first != last; ++first) {
        if (p(*first)) {
            return first;
        }
    }
    return last;
}

template<class InputIt, class T>
constexpr InputIt find(InputIt first, InputIt last, const T& value)
{
    for (; first != last; ++first) {
        if (*first == value) {
            return first;
        }
    }
    return last;
}

#endif //TEMPLATE_CRUSH_SAGA_UTILS_HPP
