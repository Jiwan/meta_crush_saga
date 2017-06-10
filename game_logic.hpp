//
// Created by jguegant on 2017-06-09.
//
#ifndef TEMPLATE_CRUSH_SAGA_GAME_ENGINE_HPP
#define TEMPLATE_CRUSH_SAGA_GAME_ENGINE_HPP

#include <cstddef>
#include <variant>

#include "board.hpp"


template <std::size_t RowCount, std::size_t ColumnCount>
class game_logic
{
public:
    CONSTEXPR game_logic(const board<RowCount, ColumnCount>& board) : board_(board)
    {
    }

    CONSTEXPR board<RowCount, ColumnCount> update()
    {
        bool board_updating = falldown() | generate();

        if (board_updating) {
            return board_;
        }

        board_updating = animate_matches();

        if (board_updating) {
            return board_;
        }

        // Take inputs in consideration!

        find_matches();

        return board_;
    }

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
                    continue;

                    any_updated |= true;
                }
            }
        }

        return any_updated;
    }

    CONSTEXPR void find_matches()
    {
        auto mark_range_as_matched = [this](int x, int y, int index, bool horizontal) constexpr { 
            for (int i = x; i < y; ++i) {
                if (horizontal) {
                    board_[index][i].state.matched = true; 
                } else {
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
    }

private:
    board<RowCount, ColumnCount> board_;
};

#endif //TEMPLATE_CRUSH_SAGA_GAME_ENGINE_HPP
