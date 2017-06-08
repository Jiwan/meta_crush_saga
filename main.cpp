#include <iostream>
#include <utility>

#include "constexpr_string.hpp"
#include "board.hpp"
#include "utils.hpp"

constexpr auto parse_board_column_count(const constexpr_string& str)
{
    bool was_space = true;
    int column_count = 0;

    for (std::size_t i = 0; i < str.size(); ++i) {
        if (str[i] == '\n') {
            break;
        } else if (str[i] == ' ') {
            was_space = true;
        } else if(was_space) {
            ++column_count;
            was_space = false;
        }
    }

    return column_count;
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

template <class BoardString, std::size_t RowIndex, std::size_t ColumnIndex, std::size_t ColumnCount>
constexpr auto parse_candy(BoardString&& board_string, size_t_t<RowIndex> row_index, size_t_t<ColumnIndex> column_index, size_t_t<ColumnCount> column_count)
{
    auto row_character_count = mul(column_count, size_t_c<3>);
    constexpr auto s = board_string();

    return candy<character_to_candytype(s[(row_index * row_character_count) + column_index * 3]), true, true>{};

}

template <class BoardString, std::size_t RowIndex, std::size_t ColumnCount, std::size_t... ColumnIndices>
constexpr auto parse_row(BoardString&& board_string, size_t_t<RowIndex> row_index, size_t_t<ColumnCount> column_count, std::index_sequence<ColumnIndices...>)
{
    return row<decltype(parse_candy(board_string, row_index, size_t_c<ColumnIndices>, column_count))...>{};
}

template <class BoardString, std::size_t ColumnCount, std::size_t... RowIndices>
constexpr auto parse_board(BoardString&& board_string, size_t_t<ColumnCount> column_count,  std::index_sequence<RowIndices...>)
{
    return board<decltype(parse_row(board_string, size_t_c<RowIndices>, column_count, make_index_sequence_v<column_count>))...>{};
}

template <class BoardString>
constexpr auto parse_board(BoardString&& board_string)
{
    return parse_board(board_string,
                       size_t_c<parse_board_column_count(board_string())>,
                       make_index_sequence_v<parse_board_row_count(board_string())>);
}

int main()
{
    auto board = parse_board([]() constexpr { return constexpr_string("R  R  R  R  R  R  R  R "); });
    return 0;
}