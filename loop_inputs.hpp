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

    return board.startswith("\n") ? board.substr(1, board.size() -1) : board;
};

#endif // META_CRUSH_SAGA_LOOP_INPUTS_HPP