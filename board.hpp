//
// Created by jguegant on 2017-06-08.
//

#ifndef TEMPLATE_CRUSH_SAGA_BOARD_HPP
#define TEMPLATE_CRUSH_SAGA_BOARD_HPP

#include <iostream>
#include <utility>

#include "utils.hpp"

template <class T, char... values>
struct convertor {
    static constexpr char map [] = {values...};

    constexpr static char encode(const T& t) {
        return map[static_cast<int>(t)];
    }

    constexpr static T decode(char c) {
        for (int i = 0; i < sizeof(map); ++i) {
            if (map[i] == c) {
                return static_cast<T>(i);
            }
        }
    }
};

enum class CandyType
{
    None = 0,
    Red,
    Green,
    Blue,
};

using candy_type_convertor = convertor<CandyType, ' ', 'R', 'G', 'B'>;

enum class CandyState
{
    None = 0,
    Pruned,
    Falling,
};

using candy_state_convertor = convertor<CandyState, ' ', 'p', 'f'>;

struct candy
{
    CandyType type;
    CandyState state;
};

template <std::size_t RowCount, std::size_t ColumnCount>
using board = std::array<std::array<candy, ColumnCount>, RowCount>;

template <std::size_t RowCount, std::size_t ColumnCount>
auto print(const board<RowCount, ColumnCount>& board) -> void {

    for (std::size_t i = 0; i < RowCount; ++i) {
        for (std::size_t j = 0; j < ColumnCount; ++j) {
            std::cout << candy_type_convertor::encode(board[i][j].type) << "  ";
        }

        std::cout << std::endl;
    }
}

template <class... T>
void print(T... objs)
{
    (print(objs), ...);
}

#endif //TEMPLATE_CRUSH_SAGA_BOARD_HPP
