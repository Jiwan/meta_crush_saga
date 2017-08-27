//
// Created by jguegant on 2017-06-10.
//

#ifndef META_CRUSH_SAGA_PARSER_HPP
#define META_CRUSH_SAGA_PARSER_HPP

#include <tuple>

#include "board.hpp"
#include "game_engine.hpp"
#include "constexpr_string_view.hpp"
#include "constexpr_string.hpp"
#include "utils.hpp"

CONSTEXPR int stoi(const constexpr_string_view& s)
{
    int result = 0;

    for (const char c : s) {
        result = result * 10 + (c - '0');
    }
    
    return result;
}

CONSTEXPR auto itos(int x)
{
    int digits = digits_amount(x);

    constexpr_string<10> result;

    for (int i = 0; i < digits; ++i) {
        result[digits - 1 - i] = (x % 10) + '0';
        x = x / 10;
    }

    return result.substr(0, digits);
}

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
constexpr int candy_type_offset = 1;

constexpr auto parse_board_column_count(const constexpr_string_view& str)
{
    const auto endline = find(str.cbegin(), str.cend(), '\n');
    return (endline - str.cbegin()) / candy_size;
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

constexpr auto isolate_board_game(auto&& str)
{
    str.erase(str.begin(), find(str.begin(), str.end(), '-'));
    str.erase(str.begin(), find(str.begin(), str.end(), '\n') + 1);

    auto it = str.begin();

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
CONSTEXPR auto parse_score(GameString&& game_string)
{
    constexpr auto str = game_string();

    auto score_begin = find(str.cbegin(), str.cend(), ':') + 2;
    auto score_end = find(score_begin, str.cend(), '\n');

    return stoi({score_begin, static_cast<int>(score_end - score_begin)});
}

template <class GameString>
CONSTEXPR auto parse_moves(GameString&& game_string)
{
    constexpr auto str = game_string();

    auto score_begin = find(str.cbegin(), str.cend(), ':') + 1;
    auto moves_begin = find(score_begin, str.cend(), ':') + 2;
    auto moves_end = find(moves_begin, str.cend(), '\n');

    return stoi({moves_begin, static_cast<int>(moves_end - moves_begin)});
}

template <class GameString>
CONSTEXPR auto parse_game(GameString&& game_string)
{
    CONSTEXPR auto board = parse_board(game_string);
    CONSTEXPR auto score = parse_score(game_string);
    CONSTEXPR auto moves = parse_moves(game_string);

    return std::make_tuple(board, score, moves);
}

template <std::size_t RowCount, std::size_t ColumnCount>
CONSTEXPR auto print_board_to_array(const board<RowCount, ColumnCount>& board)
{
    constexpr int row_padding = 5; // space begin + | + | + space end + \n
    constexpr int width = ((ColumnCount * candy_size) + row_padding);

    constexpr auto e = [](CandyState s) constexpr { return encode_candy_state(s); };

    constexpr int board_size_in_char = width * ((RowCount * candy_size) + 2);
    constexpr_string<board_size_in_char> result;

    int cursor = 0;
    
#define W(C) result[cursor++] = C
#define NEWLINE() W(' '); W('|')
#define ENDLINE() W('|'); W(' '); W('\n')

#define EPILOG() \
    W(' ');W(' '); \
    for (int r = 0; r < width - row_padding; ++r) { \
        W('-'); \
    } \
    W(' ');W(' ');W('\n')

    EPILOG();

    for (const auto& row : board) {
        NEWLINE();

        for (const auto& column : row) {
            W(' '); W(e(column.state).top); W(' ');
        }

        ENDLINE();
        NEWLINE();

        for (const auto& column : row) {
            W(e(column.state).left);
            W(encode_candy_type(column.type));
            W(e(column.state).right);
        }

        ENDLINE();
        NEWLINE();

        for (const auto& column : row) {
            W(' '); W(e(column.state).bottom); W(' ');  
        }

        ENDLINE();
    }

    EPILOG();

    return result;
}

CONSTEXPR auto print_score(auto& engine)
{
    return constexpr_string("> score: ").append(itos(engine.get_score())).append(constexpr_string("\n"));
}

CONSTEXPR auto print_moves(auto& engine)
{
    return constexpr_string("> moves: ").append(itos(engine.get_moves())).append(constexpr_string("\n"));
}

CONSTEXPR auto print_game(auto& engine)
{
    auto result = constexpr_string("      Meta crush saga      \n");
    auto board = print_board_to_array(engine.get_board());
    auto score = print_score(engine);
    auto moves = print_moves(engine);

    return result.append(board).append(score).append(moves);
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
