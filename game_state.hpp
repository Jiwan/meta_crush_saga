#ifndef META_CRUSH_SAGA_GAME_STATE_HPP
#define META_CRUSH_SAGA_GAME_STATE_HPP

#include <cstddef>

template <std::size_t RowCount, std::size_t ColumnCount>
struct GameState
{
    std::array<std::array<candy, ColumnCount>, RowCount> board;
    int hovered_x;
    int hovered_y;
    bool any_selected;
    int selected_x;
    int selected_y;
    int score;
    int moves;
};

#endif  // META_CRUSH_SAGA_GAME_STATE_HPP
