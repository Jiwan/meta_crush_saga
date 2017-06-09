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
private:
    enum class State
    {
        Pruning,
        Falling,
        Regenerating,
        PickingFirst,
        PickingSecond,
        Swapping
    };

public:
    constexpr game_logic(const board<RowCount, ColumnCount>& board) : board_(board)
    {
        state_ = get_state_from_board();
    }

    constexpr game_logic& update() {
    }

private:
    constexpr State get_state_from_board()
    {
        //

    }

private:
    State state_;
    board<RowCount, ColumnCount> board_;
};


#endif //TEMPLATE_CRUSH_SAGA_GAME_ENGINE_HPP
