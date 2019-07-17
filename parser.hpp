//
// Created by jguegant on 2017-06-10.
//

#ifndef META_CRUSH_SAGA_PARSER_HPP
#define META_CRUSH_SAGA_PARSER_HPP

#include <tuple>

#include "board.hpp"
#include "constexpr_string.hpp"
#include "constexpr_string_view.hpp"
#include "game_engine.hpp"
#include "utils.hpp"

struct BoardParameters
{
    int row_count;
    int column_count;
    int score;
    int moves;
};

constexpr int extract_int(auto it)
{
    int result = 0;
    char digit = *it;
    while ('0' <= digit && digit <= '9') {
        result = 10 * result + (digit - '0');
        ++it;
        digit = (*it);
    }
    return result;
}

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
    switch (c) {
        case ' ':
            return CandyType::None;
        case 'R':
            return CandyType::Red;
        case 'G':
            return CandyType::Green;
        case 'B':
            return CandyType::Blue;
        case 'Y':
            return CandyType::Yellow;
        default:
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

    return dec;
}

CONSTEXPR CandyState decode_candy_state(char up_char)
{
    CandyState state = { false };

    if (up_char == '*') {
        state.matched = true;
    } else if (up_char != ' ') {
        throw std::runtime_error("Invalid matched state!");
    }

    return state;
}

constexpr int candy_size = 3;
constexpr int candy_type_offset = 1;
constexpr int field_margins = 5;  // space, pipe, pipe, space, newline
constexpr int noncandy_rows = 3;  // title, border, border

constexpr auto parse_board_parameters(auto&& str)
{
    BoardParameters ret{ 0, 0, 0, 0 };

    auto gs_begin = str.cbegin();
    auto gs_end = str.cend();
    auto lefttop_corner = find(gs_begin + 1, gs_end, '\n');
    auto righttop_corner = find(lefttop_corner + 1, gs_end, '\n');
    auto field_width = righttop_corner - lefttop_corner;
    ret.column_count = (field_width - field_margins) / candy_size;

    auto score_start = find(gs_begin + 1, gs_end, '>');
    auto score_start_position = score_start - gs_begin;
    ret.row_count = ((score_start_position / field_width) - noncandy_rows) / candy_size;

    const auto score_value_offset = 9;  // length of "> score: "
    ret.score = extract_int(score_start + score_value_offset);

    auto moves_start = find(score_start + 1, gs_end, '>');
    const auto moves_value_offset = 9;  // length of "> moves: "
    ret.moves = extract_int(moves_start + moves_value_offset);

    return ret;
}

constexpr auto isolate_board_game(auto&& str)
{
    str.erase(str.begin(), find(str.begin(), str.end(), '-'));
    str.erase(str.begin(), find(str.begin(), str.end(), '\n') + 1);

    auto it = str.begin();

    while (true) {
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
CONSTEXPR auto parse_board(GameString&& get_game_state_string)
{
    constexpr auto board_string = isolate_board_game(get_game_state_string());

    constexpr auto board_parameters = parse_board_parameters(get_game_state_string());
    constexpr int column_count = board_parameters.column_count;
    constexpr int row_count = board_parameters.row_count;

    bool any_hovered = false;
    int hovered_x = 0;
    int hovered_y = 0;
    bool any_selected = false;
    int selected_x = 0;
    int selected_y = 0;

    std::array<std::array<candy, column_count>, row_count> board{};

    for (int i = 0; i < row_count; ++i) {
        for (int j = 0; j < column_count; ++j) {
            const int row_character_count = (column_count * candy_size) + 1;
            const int candy_area_index = (i * (row_character_count * candy_size)) + (j * candy_size);
            const int candy_type_index = candy_area_index + (row_character_count * candy_type_offset) + candy_type_offset;
            const int candy_state_selected_or_hover = candy_type_index - candy_type_offset;
            const int candy_matched = candy_area_index + candy_type_offset;

            board[i][j] = candy{
                decode_candy_type(board_string[candy_type_index]),
                decode_candy_state(board_string[candy_matched])
            };

            if (board_string[candy_state_selected_or_hover] == '[') {
                any_selected = true;
                selected_x = i;
                selected_y = j;
            } else if (board_string[candy_state_selected_or_hover] == '(') {
                any_hovered = true;
                hovered_x = i;
                hovered_y = j;
            } else if (board_string[candy_state_selected_or_hover] != ' ' && board_string[candy_state_selected_or_hover] != '*') {
                throw std::runtime_error("Invalid hover state!");
            }
        }
    }

    if ((!any_hovered) && any_selected) {
        hovered_x = selected_x;
        hovered_y = selected_y;
    }
    return std::make_tuple(BoardExtented<row_count, column_count>{ board, hovered_x, hovered_y, any_selected, selected_x, selected_y }, board_parameters.score, board_parameters.moves);
}

template <class GameString>
CONSTEXPR auto parse_score(GameString&& get_game_state_string)
{
    constexpr auto str = get_game_state_string();

    auto score_begin = find(str.cbegin(), str.cend(), ':') + 2;
    auto score_end = find(score_begin, str.cend(), '\n');

    return stoi({ score_begin, static_cast<int>(score_end - score_begin) });
}

template <class GameString>
CONSTEXPR auto parse_moves(GameString&& get_game_state_string)
{
    constexpr auto str = get_game_state_string();

    auto score_begin = find(str.cbegin(), str.cend(), ':') + 1;
    auto moves_begin = find(score_begin, str.cend(), ':') + 2;
    auto moves_end = find(moves_begin, str.cend(), '\n');

    return stoi({ moves_begin, static_cast<int>(moves_end - moves_begin) });
}

template <class GameString>
CONSTEXPR auto parse_game_state(GameString&& get_game_state_string)
{
    CONSTEXPR auto board = parse_board(get_game_state_string);

    return board;
}

template <std::size_t RowCount, std::size_t ColumnCount>
CONSTEXPR auto print_board_to_array(const game_engine<RowCount, ColumnCount>& engine)
{
    auto board = engine.get_board();

    constexpr int row_padding = 5;  // space begin + | + | + space end + \n
    constexpr int width = ((ColumnCount * candy_size) + row_padding);

    constexpr auto e = [](CandyState s) constexpr
    {
        return encode_candy_state(s);
    };

    constexpr int board_size_in_char = width * ((RowCount * candy_size) + 2);
    constexpr_string<board_size_in_char> result;

    int cursor = 0;

#define W(C) result[cursor++] = C
#define NEWLINE() \
    W(' ');       \
    W('|')
#define ENDLINE() \
    W('|');       \
    W(' ');       \
    W('\n')

#define EPILOG()                                    \
    W(' ');                                         \
    W(' ');                                         \
    for (int r = 0; r < width - row_padding; ++r) { \
        W('-');                                     \
    }                                               \
    W(' ');                                         \
    W(' ');                                         \
    W('\n')

    EPILOG();

    for (const auto& row : board) {
        NEWLINE();

        for (const auto& column : row) {
            W(' ');
            W(e(column.state).top);
            W(' ');
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
            W(' ');
            W(e(column.state).bottom);
            W(' ');
        }

        ENDLINE();
    }

    EPILOG();

    auto hovered_x = engine.get_hovered_x();
    auto hovered_y = engine.get_hovered_y();
    auto hovered_center = (hovered_y * candy_size + 3) + (hovered_x * candy_size + 2) * width;
    result[hovered_center - 1] = '(';
    result[hovered_center + 1] = ')';

    if (engine.is_any_selected()) {
        auto selected_x = engine.get_selected_x();
        auto selected_y = engine.get_selected_y();
        auto selected_center = (selected_y * candy_size + 3) + (selected_x * candy_size + 2) * width;
        result[selected_center - 1] = '[';
        result[selected_center + 1] = ']';
    }

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

CONSTEXPR auto print_game_state(auto& engine)
{
    auto result = constexpr_string("      Meta crush saga      \n");
    auto board = print_board_to_array(engine);
    auto score = print_score(engine);
    auto moves = print_moves(engine);

    return result.append(board).append(score).append(moves);
}

#endif  //TEMPLATE_CRUSH_SAGA_PARSER_HPP
