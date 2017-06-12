#ifndef META_CRUSH_SAGA_LOOP_INPUTS_HPP
#define META_CRUSH_SAGA_LOOP_INPUTS_HPP

#include "constexpr_string.hpp"
#include "inputs.hpp"

constexpr KeyboardInput keyboard_input = KeyboardInput::KEYBOARD_INPUT;
constexpr long long epoch_ms = EPOCH_MS;

constexpr auto board_string = []() constexpr
{
    auto board = constexpr_string(
#include "board.txt"
    );

    if (board.startswith("\n")) {
        board.erase(board.begin());
    }

    return board;
};

#endif // META_CRUSH_SAGA_LOOP_INPUTS_HPP