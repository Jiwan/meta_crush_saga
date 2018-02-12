#ifndef META_CRUSH_SAGA_LOOP_INPUTS_HPP
#define META_CRUSH_SAGA_LOOP_INPUTS_HPP

#include "constexpr_string.hpp"
#include "inputs.hpp"

constexpr KeyboardInput keyboard_input = KeyboardInput::KEYBOARD_INPUT;
constexpr long long epoch_ms = EPOCH_MS;

constexpr auto get_game_state_string = []() constexpr
{
    auto game_state_string = constexpr_string(
#include "current_state.txt"
    );

    return game_state_string;
};

#endif // META_CRUSH_SAGA_LOOP_INPUTS_HPP
