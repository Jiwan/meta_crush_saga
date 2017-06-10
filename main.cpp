#include <array>
#include <iostream>
#include <utility>
#include <variant>

#include "parser.hpp"
#include "game_logic.hpp"

constexpr auto board_string = []() constexpr
{
    auto board = constexpr_string(
R"(
                        
 R  B  G  R  B  R  R  G 
                        
                        
 B  G  G  R  G  R  R  G 
                        
                        
 R  B  G  R  B  R  R  G 
                        
                        
 B  G  G  R  G  R  R  G 
                        
                        
 R  B  B  R  R  R  R  G 
                        
                        
 R  B  G  R  B  R  R  G 
                        
)"
    );

    return board.startswith("\n") ? board.substr(1, board.size() -1) : board;
};

int main()
{
    CONSTEXPR auto board = parse_board(board_string);
    CONSTEXPR auto new_board = game_logic(board).update();

    print_board(new_board);

    return 0;
}