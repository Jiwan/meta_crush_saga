#include <array>
#include <iostream>
#include <utility>
#include <variant>

#include "loop_inputs.hpp"
#include "game_logic.hpp"
#include "parser.hpp"

int main()
{
    // parse_game(board_string);

    CONSTEXPR auto board = parse_game(board_string);
    CONSTEXPR auto new_board = game_logic(board, epoch_ms).update(keyboard_input);

    print_board_to_array(new_board);   

    return 0;
}