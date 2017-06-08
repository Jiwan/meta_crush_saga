//
// Created by jguegant on 2017-06-08.
//

#ifndef TEMPLATE_CRUSH_SAGA_BOARD_HPP
#define TEMPLATE_CRUSH_SAGA_BOARD_HPP

#include <iostream>
#include <utility>

enum class CandyType
{
    RED = 0,
    BLUE = 1,
    GREEN = 2
};

constexpr CandyType character_to_candytype(char c)
{
    if (c == 'R') {
        return CandyType::RED;
    } else if (c == 'B') {
        return CandyType::BLUE;
    } else if (c == 'G') {
        return CandyType::GREEN;
    } else {
        throw std::runtime_error("Invalid candytype");
    }
}

template <CandyType type, bool selected, bool matched>
struct candy
{
    using candy_type = std::integral_constant<CandyType, type>;
};

template <class... Candies>
struct row
{
    static void raw()
    {

    }
};

template <class... Rows>
struct board
{
    static void draw()
    {
        Rows::draw();
    }
};

#endif //TEMPLATE_CRUSH_SAGA_BOARD_HPP
