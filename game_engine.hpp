#ifndef META_CRUSH_SAGA_GAME_ENGINE_HPP
#define META_CRUSH_SAGA_GAME_ENGINE_HPP

#include <cstddef>
#include <optional>
#include <variant>
#include <tuple>

#include "board.hpp"
#include "inputs.hpp"
#include "random.hpp"


template <std::size_t RowCount, std::size_t ColumnCount>
class game_engine
{
public:
    CONSTEXPR game_engine(const std::tuple< std::array<std::array<candy, ColumnCount>, RowCount>, int, int, bool, int, int > board_extended, int score, int moves, long long epoch_ms) :
        board_(std::get<0>(board_extended)),
        score_(score),
        moves_(moves),
        rg_(static_cast<std::uint16_t>(epoch_ms)),
        hovered_x_(std::get<1>(board_extended)),
        hovered_y_(std::get<2>(board_extended)),
        any_selected_(std::get<3>(board_extended)),
        selected_x_(std::get<4>(board_extended)),
        selected_y_(std::get<5>(board_extended))
    {
    }

    CONSTEXPR game_engine& update(KeyboardInput input)
    {
        bool board_updating = falldown() | generate();

        if (board_updating) {
            return *this;
        }

        board_updating = animate_matches();

        if (board_updating) {
            return *this;
        }

        board_updating = find_matches();

        if (board_updating) {
            return *this;
        }

        handle_input(input);

        return *this;
    }

    CONSTEXPR auto get_board() const
    {
        return board_;
    }

    CONSTEXPR int get_score() const
    {
        return score_;
    }

    CONSTEXPR int get_moves() const
    {
        return moves_;
    }

    CONSTEXPR int get_hovered_x() const
    {
        return hovered_x_;
    }

    CONSTEXPR int get_hovered_y() const
    {
        return hovered_y_;
    }

    CONSTEXPR bool is_any_selected() const
    {
        return any_selected_;
    }

    CONSTEXPR int get_selected_x() const
    {
        return selected_x_;
    }

    CONSTEXPR int get_selected_y() const
    {
        return selected_y_;
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
                    board_[i + 1][j].type = board_[i][j].type; 
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
                CandyType type = static_cast<CandyType>((rg_.next() % candy_type_count) + 1);
                candy.type = type;
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
                    board_[index][i].state.matched = true; 
                } else {
                    board_[i][index].state.matched = true; 
                }

                ++score_;
            }
        };

        int same_type_counter = 1;

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

        return any_match;
    }

    CONSTEXPR void handle_input(KeyboardInput input)
    {
        int cursor_x = hovered_x_;
        int cursor_y = hovered_y_;

        auto move_cursor_relative = [=](int x, int y) constexpr {            
            if (static_cast<int>(cursor_x) + x < 0 || static_cast<int>(cursor_x) + x >= RowCount) {
                x = 0;
            }
            if (static_cast<int>(cursor_y) + y < 0 || static_cast<int>(cursor_y) + y >= ColumnCount) {
                y = 0; 
            }

            hovered_x_ +=x;
            hovered_y_ +=y;
        };

        switch (input)
        {
            case KeyboardInput::Left:
                move_cursor_relative(0, -1);                
                break;
            case KeyboardInput::Right:
                move_cursor_relative(0, 1);
                break;
            case KeyboardInput::Up:
                move_cursor_relative(-1, 0);
                break;
            case KeyboardInput::Down:
                move_cursor_relative(1, 0);
                break;
            case KeyboardInput::Space:
                if (any_selected_)
                {
                    int selected_x = selected_x_;
                    int selected_y = selected_y_;
                    auto distance_to_selected = abs(selected_x - cursor_x) + abs(selected_y - cursor_y);

                    if ((distance_to_selected) == 1) {
                        if (moves_ <= 0) {
                            break;
                        }
                        any_selected_ = false;
                        swap(board_[selected_x][selected_y], board_[cursor_x][cursor_y]);

                        if (!find_matches()) {
                            swap(board_[selected_x][selected_y], board_[cursor_x][cursor_y]);
                            any_selected_ = true;
                        } else {
                            --moves_;
                        }
                    } else if (distance_to_selected == 0) {
                        any_selected_ = false;
                    }
                } else {
                    any_selected_ = true;
                    selected_x_ = hovered_x_;
                    selected_y_ = hovered_y_;
                }

                break;
        }
    }

    CONSTEXPR std::pair<int, int> find_cursor()
    {
        auto coord = find_if([](const candy& c){ return c.state.hover; });

        if (coord) {
            return coord.value();
        }

        return find_if([](const candy& c){ return c.state.selected; }).value();
    }

    template <class Predicate>
    CONSTEXPR std::optional<std::pair<int, int>> find_if(Predicate&& p)
    {
        for (int i = 0; i < RowCount; ++i) {
            for (int j = 0; j < ColumnCount; ++j) {
                if (p(board_[i][j])) {
                    return std::make_pair(i, j); 
                }
            }
        }

        return std::nullopt;
    }

private:
    random_generator rg_;
    board<RowCount, ColumnCount> board_;
    int score_;
    int moves_;
    bool any_selected_;
    int selected_x_;
    int selected_y_;
    int hovered_x_;
    int hovered_y_;
};

#endif //TEMPLATE_CRUSH_SAGA_GAME_ENGINE_HPP
