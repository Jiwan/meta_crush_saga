//
// Created by jguegant on 2017-06-08.
//

#ifndef TEMPLATE_CRUSH_SAGA_UTILS_HPP
#define TEMPLATE_CRUSH_SAGA_UTILS_HPP

#include <utility>

template <int i>
constexpr std::integral_constant<int, i> int_c{};

template <int i>
using int_t = std::integral_constant<int, i>;

template <std::size_t i>
constexpr std::integral_constant<std::size_t, i> size_t_c{};

template <std::size_t i>
using size_t_t = std::integral_constant<std::size_t, i>;

template <std::size_t N>
constexpr std::make_index_sequence<N> make_index_sequence_v{};

template <std::size_t X, std::size_t Y>
constexpr auto mul(size_t_t<X>, size_t_t<Y>)
{
    return size_t_t<X * Y>{};
};

#endif //TEMPLATE_CRUSH_SAGA_UTILS_HPP
