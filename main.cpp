#include <array>
#include <iostream>
#include <utility>
#include <variant>

#include "constexpr_string.hpp"
#include "board.hpp"
#include "game_logic.hpp"
#include "utils.hpp"

constexpr auto parse_board_column_count(const constexpr_string& str)
{
    std::size_t i = 0;

    for (; i < str.size(); ++i) {
        if (str[i] == '\n') {
            break;
        }
    }

    return i / 3;
}

constexpr auto parse_board_row_count(const constexpr_string& str)
{
    int row_count = 1;

    for (std::size_t i = 0; i < str.size(); ++i) {
        if (str[i] == '\n') {
            ++row_count;
        }
    }

    return row_count;
}

template <class BoardString>
constexpr auto parse_candy(BoardString&& board_string,
                           std::size_t row_index,
                           std::size_t column_index,
                           std::size_t column_count)
{
    constexpr auto s = board_string();
    std::size_t row_character_count = column_count * 3;

    return candy {
        candy_type_convertor::decode(s[(row_index * row_character_count) + column_index * 3]),
        candy_state_convertor::decode(s[(row_index * row_character_count) + (column_index * 3) + 1])
    };

}

template <class BoardString>
constexpr auto parse_board(BoardString&& board_string)
{
    constexpr std::size_t column_count = parse_board_column_count(board_string());
    constexpr std::size_t row_count = parse_board_row_count(board_string());

    std::array<std::array<candy, column_count>, row_count> board{};

    for (std::size_t i = 0; i < row_count; ++i) {
        for (std::size_t j = 0; j < column_count; ++j) {
            board[i][j] = parse_candy(board_string, i, j, column_count);
        }
    }

    return board;
}

int main()
{
    constexpr auto board = parse_board([]() constexpr { return constexpr_string("R  B  G  R  R  R  R  R  "); });

    auto gl = game_logic(board).update();
    print(board);

    return 0;
}