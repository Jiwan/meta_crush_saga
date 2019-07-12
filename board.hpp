//
// Created by jguegant on 2017-06-08.
//

#ifndef META_CRUSH_SAGA_BOARD_HPP
#define META_CRUSH_SAGA_BOARD_HPP

#include <array>
#include <iostream>
#include <utility>

#include "utils.hpp"

enum class CandyType
{
    None = 0,
    Red,
    Green,
    Blue,
    Yellow
};

constexpr int candy_type_count = 4;  // Excluding None.

struct CandyState
{
    bool matched;
};

struct candy
{
    CandyType type;
    CandyState state;
};

template <int RowCount, int ColumnCount>
using board = std::array<std::array<candy, ColumnCount>, RowCount>;

#endif  //TEMPLATE_CRUSH_SAGA_BOARD_HPP
