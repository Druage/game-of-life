#include <set>
#include "../../GameOfLife_T.h"

namespace test {

    const int WIDTH = 5;
    const int HEIGHT = 5;

    using GameOfLife = GameOfLife_T<WIDTH, HEIGHT>;

    template<typename TExpected>
    bool containsAll(GameOfLife &state, TExpected expected) {
        for (auto row: state.gameGrid) {
            for (auto cell: row) {
                if (cell != expected) {
                    return false;
                }
            }
        }

        return true;
    }

    bool containsOnly(GameOfLife &state, std::set<std::pair<int, int>> values) {

        int sum = 0;
        for (int w = 0; w < WIDTH; ++w) {
            for (int h = 0; h < HEIGHT; ++h) {
                const auto cell = state.gameGrid[w][h];
                if (cell) {
                    const auto containsCellInValues = values.find(std::pair<int, int>(w, h)) != values.end();
                    if (!containsCellInValues) {
                        return false;
                    }
                    sum += cell;
                }
            }
        }

        return sum == values.size();
    }


}