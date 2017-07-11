#ifndef META_CRUSH_SAGA_LOOP_INPUTS_HPP
#define META_CRUSH_SAGA_LOOP_INPUTS_HPP

#include "constexpr_string.hpp"
#include "inputs.hpp"

constexpr KeyboardInput keyboard_input = KeyboardInput::KEYBOARD_INPUT;
constexpr long long epoch_ms = EPOCH_MS;

constexpr auto game_string = []() constexpr
{
    auto game = constexpr_string(
#include "game.txt"
    );

    return game;
};

#endif // META_CRUSH_SAGA_LOOP_INPUTS_HPP