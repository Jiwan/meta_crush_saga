#ifndef TEMPLATE_CRUSH_SAGA_GAME_ENGINE_HPP
#define TEMPLATE_CRUSH_SAGA_GAME_ENGINE_HPP

#include <cstddef>
#include <variant>

#include "board.hpp"
#include "inputs.hpp"


template <std::size_t RowCount, std::size_t ColumnCount>
class game_logic
{
public:
    CONSTEXPR game_logic(const board<RowCount, ColumnCount>& board) : board_(board)
    {
    }

    CONSTEXPR board<RowCount, ColumnCount> update(KeyboardInput input)
    {
        bool board_updating = falldown() | generate();

        if (board_updating) {
            return board_;
        }

        board_updating = animate_matches();

        if (board_updating) {
            return board_;
        }

        board_updating = find_matches();

        if (board_updating) {
            return board_;
        }

        handle_input(input);

        return board_;
    }

private:

    CONSTEXPR bool falldown()
    {
        bool any_falldown = false;

        for (int j = 0; j < ColumnCount; ++j) {
            bool has_gap = false;
            
            for (int i = RowCount - 1; i >= 0 ; --i) {
                if (!has_gap && board_[i][j].type == CandyType::None) {
                    has_gap = true;
                } else if (has_gap) {
                    board_[i + 1][j] = board_[i][j]; 
                }
            }

            if (has_gap) {
                board_[0][j].type = CandyType::None;
            }

            any_falldown |= has_gap;
        }

        return any_falldown;
    }

    CONSTEXPR bool generate()
    {
        bool any_generated = false;

        for (auto& candy : board_[0]) {
            if (candy.type == CandyType::None) {
                candy.type = CandyType::Blue;
                any_generated = true;
            }
        }

        return any_generated;
    }

    CONSTEXPR bool animate_matches()
    {
        bool any_updated = false;

        for (auto& row : board_) {
            for (auto& candy : row) {
                if (candy.state.matched) {
                    candy.type = CandyType::None;
                    candy.state.matched = false;

                    any_updated |= true;
                }
            }
        }

        return any_updated;
    }

    CONSTEXPR bool find_matches()
    {
        bool any_match = false;

        auto mark_range_as_matched = [this, &any_match](int x, int y, int index, bool horizontal) constexpr {
            for (int i = x; i < y; ++i) {
                any_match |= true;

                if (horizontal) {
                    // std::cout << "Matched " << index << "," << i << std::endl; 
                    board_[index][i].state.matched = true; 
                } else {
                    // std::cout << "Matched " << i << "," << index << std::endl; 
                    board_[i][index].state.matched = true; 
                }
            }
        };

        std::size_t same_type_counter = 1;

        // Horizontal scan.

        for (int i = 0; i < RowCount; ++i) {
            same_type_counter = 1;
            CandyType last_type = CandyType::None;

            for (int j = 0; j < ColumnCount; ++j) {
                const auto& candy = board_[i][j]; 

                if (candy.type == last_type) {
                    ++same_type_counter;
                } else {
                    if(same_type_counter >= 3) {
                        mark_range_as_matched(j - same_type_counter, j, i, true);
                    }
                    
                    same_type_counter = 1;
                }

                last_type = candy.type;
            }

            if(same_type_counter >= 3) {
                mark_range_as_matched(ColumnCount - same_type_counter, ColumnCount, i, true);
            }
        }

        // Vertical scan.

        for (int i = 0; i < ColumnCount; ++i) {
            same_type_counter = 1;
            CandyType last_type = CandyType::None;

            for (int j = 0; j < RowCount; ++j) {
                const auto& candy = board_[j][i]; 

                if (candy.type == last_type) {
                    ++same_type_counter;
                } else {
                    // std::cout << "j: " << j << std::endl;
                    // std::cout << "count: " << same_type_counter << std::endl;
                    if(same_type_counter >= 3) {
                        mark_range_as_matched(j - same_type_counter, j, i, false);
                    }
                    
                    same_type_counter = 1;
                }

                last_type = candy.type;
            }

            if(same_type_counter >= 3) {
                mark_range_as_matched(RowCount - same_type_counter, RowCount, i, false);
            }
        }

        return any_match;
    }

    CONSTEXPR void handle_input(KeyboardInput input)
    {

    }

private:
    board<RowCount, ColumnCount> board_;
};

#endif //TEMPLATE_CRUSH_SAGA_GAME_ENGINE_HPP
