#include <array>
#include <iostream>
#include <utility>
#include <variant>

#include "loop_inputs.hpp"
#include "game_engine.hpp"
#include "parser.hpp"

int main()
{
    CONSTEXPR auto game = parse_game(get_game_state_string);
    CONSTEXPR auto engine = game_engine(std::get<0>(game), std::get<1>(game), std::get<2>(game), epoch_ms)
                                   .update(keyboard_input);

    CONSTEXPR auto array = print_game(engine);

    for (const char& c : array) {
        if (c == '\0')
            std::cout << "<< here";
            
        std::cout << c;
    }

    return 0;
}
