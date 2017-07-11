#include <array>
#include <iostream>
#include <utility>
#include <variant>

#include "loop_inputs.hpp"
#include "game_engine.hpp"
#include "parser.hpp"

int main()
{
    // parse_game(board_string);

    CONSTEXPR auto game = parse_game(game_string);
    CONSTEXPR auto engine = game_engine(std::get<0>(game), std::get<1>(game), std::get<2>(game), epoch_ms)
                                   .update(keyboard_input);


    CONSTEXPR auto array = print_game(engine);

    for (const char& c : array) {
        std::cout << c;
    }

    std::cout << std::endl;
    std::cout << "score: " << std::get<1>(game);


    return 0;
}