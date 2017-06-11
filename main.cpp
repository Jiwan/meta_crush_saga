#include <array>
#include <iostream>
#include <utility>
#include <variant>

#include "loop_inputs.hpp"
#include "game_logic.hpp"
#include "parser.hpp"

int main()
{
    CONSTEXPR auto board = parse_board(board_string);
    CONSTEXPR auto new_board = game_logic(board).update(keyboard_input);

    print_board(new_board);

    return 0;
}