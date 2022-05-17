#pragma once

#include <iostream>
#include <vector>
#include <array>
#include <functional>

template<int TRows, int TCols>
class GameOfLife_T {

    using CollectedCells = std::vector<std::pair<int, int>>;
    int m_rows{TRows};
    int m_cols{TCols};

public:
    std::array<std::array<int, TRows>, TCols> gameGrid;

    void seed(std::initializer_list<std::pair<int, int>> values);

    void tick();

    void print();

public:
    std::array<int, TRows> &operator[](int row);

    [[nodiscard]] int size() const;

    [[nodiscard]] int rows() const;

    [[nodiscard]] int cols() const;

private:
    inline bool cellIsOverpopulated(int cellSum);

    inline bool cellIsUnderpopulated(int cellSum);

    inline void setCellsSimultaneously(CollectedCells &cellsToChange, int value);

    inline bool cellIsAtEdge(int row, int col);

    inline void
    collectCellsIf(int row, int col, CollectedCells &destination, std::function<bool(int)> &&collectCondition);
};

template<int TWidth, int THeight>
void GameOfLife_T<TWidth, THeight>::seed(std::initializer_list<std::pair<int, int>> values) {
    for (const auto &coords: values) {
        auto x = coords.first;
        auto y = coords.second;
        gameGrid[x][y] = 1;
    }
}

template<int TWidth, int THeight>
void GameOfLife_T<TWidth, THeight>::tick() {

#ifdef LOG_STATE
    std::cout << "Before:" << std::endl;
    print();
    std::cout << std::endl;
#endif

    CollectedCells cellsToKill;
    CollectedCells cellsBorn;

    for (int row = 0; row < TWidth; ++row) {
        for (int col = 0; col < THeight; ++col) {

            if (gameGrid[row][col]) {

                if (cellIsAtEdge(row, col)) {
                    cellsToKill.emplace_back(row, col);
                } else {
                    collectCellsIf(row, col, cellsToKill, [&](int cellSum) -> bool {
                        return cellIsUnderpopulated(cellSum) || cellIsOverpopulated(cellSum);
                    });
                }

            } else {
                collectCellsIf(row, col, cellsBorn, [&](int cellSum) -> bool {
                    return cellSum == 3;
                });
            }

        }
    }

    setCellsSimultaneously(cellsToKill, 0);
    setCellsSimultaneously(cellsBorn, 1);

#ifdef LOG_STATE
    std::cout << "After:" << std::endl;
    print();
    std::cout << std::endl;
#endif
}

template<int TWidth, int THeight>
void GameOfLife_T<TWidth, THeight>::print() {
    for (int w = 0; w < TWidth; ++w) {
        for (int h = 0; h < THeight; ++h) {
            std::cout << gameGrid[w][h] << " ";
        }
        std::cout << std::endl;
    }
}

template<int TWidth, int THeight>
bool GameOfLife_T<TWidth, THeight>::cellIsOverpopulated(int cellSum) {
    return cellSum > 3;
}

template<int TWidth, int THeight>
bool GameOfLife_T<TWidth, THeight>::cellIsUnderpopulated(int cellSum) {
    return cellSum < 2;
}

template<int TWidth, int THeight>
void GameOfLife_T<TWidth, THeight>::setCellsSimultaneously(CollectedCells &cellsToChange, int value) {
    for (auto cell: cellsToChange) {
        gameGrid[cell.first][cell.second] = value;
    }
}

template<int TWidth, int THeight>
bool GameOfLife_T<TWidth, THeight>::cellIsAtEdge(int row, int col) {
    return (row == 0 || col == 0 || row == TWidth - 1 || col == THeight - 1);
}

template<int TWidth, int THeight>
void GameOfLife_T<TWidth, THeight>::collectCellsIf(int row, int col, GameOfLife_T::CollectedCells &destination,
                                                   std::function<bool(int)> &&collectCondition) {

    const int rightCell = gameGrid[row + 1][col];
    const int leftCell = gameGrid[row - 1][col];
    const int topCell = gameGrid[row][col - 1];
    const int bottomCell = gameGrid[row][col + 1];

    const int topLeftCell = gameGrid[row - 1][col - 1];
    const int topRightCell = gameGrid[row + 1][col - 1];
    const int bottomLeftCell = gameGrid[row - 1][col + 1];
    const int bottomRightCell = gameGrid[row + 1][col + 1];

    auto cellSum = (rightCell + leftCell + topCell + bottomCell
                    + topLeftCell + topRightCell + bottomLeftCell +
                    bottomRightCell);

    if (collectCondition(cellSum)) {
        destination.emplace_back(row, col);
    }
}

template<int TRows, int TCols>
std::array<int, TRows> &GameOfLife_T<TRows, TCols>::operator[](int row) {
    return gameGrid[row];
}

template<int TRows, int TCols>
int GameOfLife_T<TRows, TCols>::size() const {
    return m_rows * m_cols;
}

template<int TRows, int TCols>
int GameOfLife_T<TRows, TCols>::rows() const {
    return m_rows;
}

template<int TRows, int TCols>
int GameOfLife_T<TRows, TCols>::cols() const {
    return m_cols;
}
