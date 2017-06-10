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
                        
 R  B  G  R  R  R  R  R 
                        
                        
 R  B  G  R  R xR  R  R 
                        
                        
 R  B  G  R  R [R  R  R 
                        
                        
 R  B  G  R  R  R  R  R 
                        
                        
 R  B *G  R  R (R  R  R 
                        
                        
 R  B  G  R  R  R  R  R 
                        
)"
    );

    return board.startswith("\n")? board.substr(1, board.size() -1): board;
};

int main()
{
    CONSTEXPR auto board = parse_board(board_string);

    auto gl = game_logic(board).update();
    print_board(board);

    return 0;
}