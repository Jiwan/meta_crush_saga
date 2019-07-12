#include <array>
#include <iostream>
#include <utility>
#include <variant>

#include "game_engine.hpp"
#include "loop_inputs.hpp"
#include "parser.hpp"

int main()
{
    CONSTEXPR auto current_state = parse_game_state(get_game_state_string);
    CONSTEXPR auto updated_engine = game_engine(std::get<0>(current_state), std::get<1>(current_state), std::get<2>(current_state), epoch_ms)
                                        .update(keyboard_input);

    CONSTEXPR auto array = print_game_state(updated_engine);

    for (const char& c : array) {
        std::cout << c;
    }

    return 0;
}
