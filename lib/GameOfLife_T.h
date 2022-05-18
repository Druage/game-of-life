#pragma once

#include <array>
#include <functional>
#include <iostream>
#include <vector>

template<int TRows, int TCols, typename T = uint8_t>
class GameOfLife_T {

    using CollectedCells = std::vector<std::pair<int, int>>;
    int m_rows{TRows};
    int m_cols{TCols};

public:
    GameOfLife_T();

    std::array<std::array<T, TRows>, TCols> gameGrid;

    void seed(std::initializer_list<std::pair<int, int>> values);

    void tick();

    void print();

    [[nodiscard]] const T *data() const {
        return gameGrid.data()->data();
    }

public:
    std::array<T, TRows> &operator[](int row);

    [[nodiscard]] int size() const;

    [[nodiscard]] int rows() const;

    [[nodiscard]] int cols() const;

    T value_type;
private:
    inline bool cellIsOverpopulated(int cellSum);

    inline bool cellIsUnderpopulated(int cellSum);

    inline void setCellsSimultaneously(CollectedCells &cellsToChange, T value);

    inline bool cellIsAtEdge(int row, int col);

    inline void collectCellsIf(int row, int col, CollectedCells &destination,
                               std::function<bool(int)> &&collectCondition);
};

template<int TWidth, int THeight, typename T>
void GameOfLife_T<TWidth, THeight, T>::seed(
        std::initializer_list<std::pair<int, int>> values) {
    for (const auto &coords: values) {
        auto x = coords.first;
        auto y = coords.second;
        gameGrid[x][y] = 1;
    }
}

template<int TWidth, int THeight, typename T>
void GameOfLife_T<TWidth, THeight, T>::tick() {

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
                        return cellIsUnderpopulated(cellSum) ||
                               cellIsOverpopulated(cellSum);
                    });
                }

            } else {
                collectCellsIf(row, col, cellsBorn,
                               [&](int cellSum) -> bool { return cellSum == 3; });
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

template<int TWidth, int THeight, typename T>
void GameOfLife_T<TWidth, THeight, T>::print() {
    for (int w = 0; w < TWidth; ++w) {
        for (int h = 0; h < THeight; ++h) {
            std::cout << static_cast<int>(gameGrid[w][h]) << " ";
        }
        std::cout << std::endl;
    }
}

template<int TWidth, int THeight, typename T>
bool GameOfLife_T<TWidth, THeight, T>::cellIsOverpopulated(int cellSum) {
    return cellSum > 3;
}

template<int TWidth, int THeight, typename T>
bool GameOfLife_T<TWidth, THeight, T>::cellIsUnderpopulated(int cellSum) {
    return cellSum < 2;
}

template<int TWidth, int THeight, typename T>
void GameOfLife_T<TWidth, THeight, T>::setCellsSimultaneously(
        CollectedCells &cellsToChange, T value) {
    for (auto cell: cellsToChange) {
        gameGrid[cell.first][cell.second] = value;
    }
}

template<int TWidth, int THeight, typename T>
bool GameOfLife_T<TWidth, THeight, T>::cellIsAtEdge(int row, int col) {
    return (row == 0 || col == 0 || row == TWidth - 1 || col == THeight - 1);
}

template<int TWidth, int THeight, typename T>
void GameOfLife_T<TWidth, THeight, T>::collectCellsIf(
        int row, int col, GameOfLife_T::CollectedCells &destination,
        std::function<bool(int)> &&collectCondition) {

    const int rightCell = gameGrid[row + 1][col];
    const int leftCell = gameGrid[row - 1][col];
    const int topCell = gameGrid[row][col - 1];
    const int bottomCell = gameGrid[row][col + 1];

    const int topLeftCell = gameGrid[row - 1][col - 1];
    const int topRightCell = gameGrid[row + 1][col - 1];
    const int bottomLeftCell = gameGrid[row - 1][col + 1];
    const int bottomRightCell = gameGrid[row + 1][col + 1];

    auto cellSum = (rightCell + leftCell + topCell + bottomCell + topLeftCell +
                    topRightCell + bottomLeftCell + bottomRightCell);

    if (collectCondition(cellSum)) {
        destination.emplace_back(row, col);
    }
}

template<int TRows, int TCols, typename T>
std::array<T, TRows> &GameOfLife_T<TRows, TCols, T>::operator[](int row) {
    return gameGrid[row];
}

template<int TRows, int TCols, typename T>
int GameOfLife_T<TRows, TCols, T>::size() const {
    return m_rows * m_cols;
}

template<int TRows, int TCols, typename T>
int GameOfLife_T<TRows, TCols, T>::rows() const {
    return m_rows;
}

template<int TRows, int TCols, typename T>
int GameOfLife_T<TRows, TCols, T>::cols() const {
    return m_cols;
}

template<int TRows, int TCols, typename T>
GameOfLife_T<TRows, TCols, T>::GameOfLife_T() {
    for (int row = 0; row < TRows; ++row) {
        for (int col = 0; col < TCols; ++col) {
            gameGrid[row][col] = static_cast<T>(1);
        }
    }
}
