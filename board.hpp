//
// Created by jguegant on 2017-06-08.
//

#ifndef TEMPLATE_CRUSH_SAGA_BOARD_HPP
#define TEMPLATE_CRUSH_SAGA_BOARD_HPP

#include <iostream>
#include <utility>

#include "utils.hpp"

enum class CandyType
{
    None = 0,
    Red,
    Green,
    Blue,
};

struct CandyState
{
    bool matched;
    bool selected;
    bool hover;
};

struct candy
{
    CandyType type;
    CandyState state;
};

template <std::size_t RowCount, std::size_t ColumnCount>
using board = std::array<std::array<candy, ColumnCount>, RowCount>;

#endif //TEMPLATE_CRUSH_SAGA_BOARD_HPP
