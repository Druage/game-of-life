#include <set>
#include "../../GameOfLife_T.h"

namespace test {

    template<typename TGrid, typename TExpected>
    bool containsAll(TGrid &grid, TExpected expected) {

        for (int row = 0; row < grid.rows(); ++row) {
            for (int col = 0; col < grid.cols(); ++col) {
                if (grid[row][col] != expected) {
                    return false;
                }

            }

        }

        return true;
    }

    template<typename TGrid>
    bool containsOnly(TGrid &grid, std::set<std::pair<int, int>> &&values) {

        int sum = 0;
        for (int r = 0; r < grid.rows(); ++r) {
            for (int c = 0; c < grid.cols(); ++c) {
                const auto cell = grid.gameGrid[r][c];
                if (cell) {
                    const auto containsCellInValues = values.find(std::pair<int, int>(r, c)) != values.end();
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