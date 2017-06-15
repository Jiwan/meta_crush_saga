//
// Created by jguegant on 2017-06-10.
//

#ifndef META_CRUSH_SAGA_PARSER_HPP
#define META_CRUSH_SAGA_PARSER_HPP

#include "board.hpp"
#include "constexpr_string_view.hpp"
#include "constexpr_string.hpp"
#include "utils.hpp"

constexpr char map_candy_type[] = { ' ', 'R', 'G', 'B', 'Y' };

constexpr char encode_candy_type(CandyType t)
{
    return map_candy_type[static_cast<int>(t)];
}

constexpr CandyType decode_candy_type(char c)
{
    int i = 0;

    for (; i < sizeof(map_candy_type); ++i) {
        if (map_candy_type[i] == c) {
            return static_cast<CandyType>(i);
        }
    }

    if (i == sizeof(map_candy_type)) {
        throw std::runtime_error("Invalid candy type");
    }
}

struct state_decoration
{
    char left;
    char right;
    char top;
    char bottom;
};

constexpr state_decoration encode_candy_state(CandyState s)
{
    state_decoration dec = { ' ', ' ', ' ', ' ' };

    if (s.matched) {
        dec = { '*', '*', '*', '*' };
    }

    if (s.selected) {
        dec.left = '[';
        dec.right = ']';
    } else if (s.hover) {
        dec.left = '(';
        dec.right = ')';
    }

    return dec;
}

CONSTEXPR CandyState decode_candy_state(char left_char, char up_char)
{
    CandyState state = { false, false , false };

    if (up_char == '*') {
        state.matched = true;
    } else if (up_char != ' ') {
        throw std::runtime_error("Invalid matched state!");
    }

    if (left_char == '[') {
        state.selected = true;
    } else if (left_char == '(') {
        state.hover = true;
    } else if (left_char != ' ' && left_char != '*') {
        throw std::runtime_error("Invalid hover state!");
    } 

    return state;
}

constexpr int candy_size = 3;

constexpr int candy_state_offset = 0;
constexpr int candy_type_offset = 1;

constexpr auto parse_board_column_count(const constexpr_string_view& str)
{
    int i = 0;

    for (; i < str.size(); ++i) {
        if (str[i] == '\n') {
            break;
        }
    }

    return i / candy_size;
}

constexpr auto parse_board_row_count(const constexpr_string_view& str)
{   
    int row_count = count(str.cbegin(), str.cend(), '\n');
    
    if (str[str.size() - 1] != '\n')
        ++row_count;

    return row_count / candy_size;
}

CONSTEXPR auto parse_candy(const constexpr_string_view& str,
                           int row_index,
                           int column_index,
                           int column_count)
{
    const int row_character_count = (column_count * candy_size) + 1;
    
    const int candy_area_index = (row_index * (row_character_count * candy_size)) +
                                 (column_index * candy_size);

    const int candy_type_index = candy_area_index + 
                                 (row_character_count * candy_type_offset) + 
                                 candy_type_offset;
    
    const int candy_state_selected_or_hover = candy_type_index - candy_type_offset;

    const int candy_matched = candy_area_index + candy_type_offset;

    return candy {
        decode_candy_type(str[candy_type_index]),
        decode_candy_state(str[candy_state_selected_or_hover], str[candy_matched])
    };

}

constexpr auto isolate_board_game(auto str)
{
    str.erase(str.begin(), find(str.begin(), str.end(), '-'));
    str.erase(str.begin(), find(str.begin(), str.end(), '\n') + 1);

    typename decltype(str)::iterator it = str.begin();

    while (true)
    {
        auto board_left = find(it, str.end(), '|');
        
        if (board_left == str.end())
            break;

        it = str.erase(it, board_left + 1);

        auto board_right = find(it, str.end(), '|');
        auto endline = find(it, str.end(), '\n');
        
        it = str.erase(board_right, endline);
        
        ++it;
    }

    str.erase(it, str.end());
    
    return str;
}

template <class GameString>
CONSTEXPR auto parse_board(GameString&& game_string)
{
    constexpr auto board_string = isolate_board_game(game_string()); 

    constexpr int column_count = parse_board_column_count(board_string);
    constexpr int row_count = parse_board_row_count(board_string);

    std::array<std::array<candy, column_count>, row_count> board{};

    for (int i = 0; i < row_count; ++i) {
        for (int j = 0; j < column_count; ++j) {
            board[i][j] = parse_candy(board_string, i, j, column_count);
        }
    }

    return board;
}

template <class GameString>
CONSTEXPR auto parse_game(GameString&& game_string)
{
    return parse_board(game_string);
}


template <std::size_t RowCount, std::size_t ColumnCount>
constexpr void print_board(const board<RowCount, ColumnCount>& board) {

    constexpr auto e = [](CandyState s) constexpr { return encode_candy_state(s); };

    for (const auto& row : board) {
        for (const auto& column : row) {
            std::cout << ' ' << e(column.state).top << ' '; 
        }

        std::cout << std::endl;

        for (const auto& column : row) {
            std::cout << e(column.state).left;
            std::cout << encode_candy_type(column.type);
            std::cout << e(column.state).right;
        }

        std::cout << std::endl;

        for (const auto& column : row) {
            std::cout << ' ' << e(column.state).bottom << ' ';  
        }

        std::cout << std::endl;
    }
}

#endif //TEMPLATE_CRUSH_SAGA_PARSER_HPP
