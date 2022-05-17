#define CATCH_CONFIG_MAIN
#define LOG_STATE

#include "catch.hpp"
#include "../GameOfLife_T.h"

/*
 * 0 | 0 0 0 0 0
 * 1 | 0 0 0 0 0
 * 2 | 0 0 0 0 0
 * 3 | 0 0 0 0 0
 * 4 | 0 0 0 0 0
 *   -----------
 *     0 1 2 3 4
 */

const int WIDTH = 5;
const int HEIGHT = 5;

using GameOfLife = GameOfLife_T<WIDTH, HEIGHT>;

bool containsAll(GameOfLife &state, int expected) {
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

TEST_CASE("The grid is the correct size and initialized to 0") {
    GameOfLife gameOfLife{};

    REQUIRE(gameOfLife.gameGrid.size() * gameOfLife.gameGrid.front().size() == WIDTH * HEIGHT);
    REQUIRE(containsAll(gameOfLife, 0));
}

/*
 * 0 | 0 0 0 0 0
 * 1 | 0 1 1 0 0
 * 2 | 0 0 0 0 0
 * 3 | 0 0 0 0 0
 * 4 | 0 0 0 0 0
 *   -----------
 *     0 1 2 3 4
 */

TEST_CASE("The grid can be seeded") {
    GameOfLife gameOfLife{};


    gameOfLife.seed({
                            {1, 1},
                            {2, 1}
                    });

    REQUIRE(gameOfLife.gameGrid[1][1] == 1);
    REQUIRE(gameOfLife.gameGrid[2][1] == 1);

    REQUIRE(containsOnly(gameOfLife, {{1, 1},
                                      {2, 1}}));
}

/*
 * 0 | 0 0 0 0 0
 * 1 | 0 1 1 0 0
 * 2 | 0 0 0 0 0
 * 3 | 0 0 0 0 0
 * 4 | 0 0 0 0 0
 *   -----------
 *     0 1 2 3 4
 */


TEST_CASE("The Game Of Life", "[Rules]") {
    GameOfLife gameOfLife{};


    SECTION("seeding only 2 adjacent cells will die", "[Underpopulation]") {
/*
 * 0 | 0 0 0 0 0
 * 1 | 0 1 1 0 0
 * 2 | 0 0 0 0 0
 * 3 | 0 0 0 0 0
 * 4 | 0 0 0 0 0
 *   -----------
 *     0 1 2 3 4
 */
        gameOfLife.seed({
                                {1, 1},
                                {2, 1}
                        });
        gameOfLife.tick();
        REQUIRE(containsAll(gameOfLife, 0));
    }

    SECTION("seeding 3 adjacent will live, 1 will die", "[Underpopulation & Live]") {
/*
 *   0 | 0 0 0 0 0
 *   1 | 0 1 0 0 0
 *   2 | 0 0 1 1 0
 *   3 | 0 0 1 0 0
 *   4 | 0 0 0 0 0
 *     -----------
 *       0 1 2 3 4
 */

        gameOfLife.seed({
                                {1, 1},
                                {2, 2},
                                {3, 2},
                                {2, 3},
                        });

        gameOfLife.tick();
        REQUIRE(containsOnly(gameOfLife, {
                {2, 1},

                {1, 2},
                {2, 2},
                {3, 2},

                {2, 3},
                {3, 3},
        }));
    }

        /*
     *   0 | 0 0 0 0 0
     *   1 | 0 1 1 0 0
     *   2 | 0 1 1 1 0
     *   3 | 0 0 1 1 0
     *   4 | 0 0 0 0 0
     *     -----------
     *       0 1 2 3 4
     */


    SECTION("seeding 3 adjacent cells will live", "[Live]") {
/*
 *   0 | 0 0 0 0 0
 *   1 | 0 1 1 0 0
 *   2 | 0 1 0 0 0
 *   3 | 0 0 0 1 0
 *   4 | 0 0 0 0 0
 *     -----------
 *       0 1 2 3 4
 */

        gameOfLife.seed({
                                {1, 1},
                                {2, 1},
                                {1, 2},
                        });

        gameOfLife.tick();
        REQUIRE(containsOnly(gameOfLife, {
                {1, 1},
                {2, 1},
                {1, 2},
                {2, 2},
        }));
    }

        /*
     *   0 | 0 0 0 0 0
     *   1 | 0 1 1 0 0
     *   2 | 0 1 1 0 0
     *   3 | 0 0 0 0 0
     *   4 | 0 0 0 0 0
     *     -----------
     *       0 1 2 3 4
     */

    SECTION("seeding 4 diagonally adjacent cells will live", "[Live]") {
/*
 *   0 | 0 0 0 0 0
 *   1 | 0 0 1 0 0
 *   2 | 0 1 0 1 0
 *   3 | 0 0 1 0 0
 *   4 | 0 0 0 0 0
 *     -----------
 *       0 1 2 3 4
 */

        gameOfLife.seed({
                                {2, 1},
                                {1, 2},
                                {3, 2},
                                {2, 3},
                        });

        gameOfLife.tick();
        REQUIRE(containsOnly(gameOfLife, {
                {2, 1},
                {1, 2},
                {3, 2},
                {2, 3},
        }));

    }

    SECTION("seeding 5 diagonally & horizontally adjacent cells will live", "[Live]") {
/*
 *   0 | 0 0 0 0 0
 *   1 | 0 1 1 0 0
 *   2 | 0 1 0 1 0
 *   3 | 0 0 1 0 0
 *   4 | 0 0 0 0 0
 *     -----------
 *       0 1 2 3 4
 */

        gameOfLife.seed({
                                {1, 1},
                                {2, 1},
                                {1, 2},
                                {3, 2},
                                {2, 3},
                        });

        gameOfLife.tick();
        REQUIRE(containsOnly(gameOfLife, {
                {1, 1},
                {2, 1},
                {1, 2},
                {3, 2},
                {2, 3},
        }));

    }

    SECTION("Overpopulation causes 3 cells to die, the remaining 3 live and reproduce",
            "[Overpopulation & Live & Reproduction]") {
/*
 *   0 | 0 0 0 0 0
 *   1 | 0 1 1 0 0
 *   2 | 0 1 1 1 0
 *   3 | 0 0 1 0 0
 *   4 | 0 0 0 0 0
 *     -----------
 *       0 1 2 3 4
 */


        gameOfLife.seed({
                                {1, 1},
                                {2, 1},
                                {1, 2},
                                {2, 2},
                                {3, 2},
                                {2, 3},
                        });

        gameOfLife.tick();
        REQUIRE(containsOnly(gameOfLife, {
                {1, 1},
                {3, 1},
                {3, 2},
                {1, 3},
                {2, 3},
                {3, 3},
        }));

        /*
 *   0 | 0 0 0 0 0
 *   1 | 0 1 0 1 0
 *   2 | 0 0 0 1 0
 *   3 | 0 1 1 1 0
 *   4 | 0 0 0 0 0
 *     -----------
 *       0 1 2 3 4
 */

    }

    SECTION("cells will die if they reach the end of the grid", "[Bounds Check]") {
/*
 *   0 | 1 1 0 0 0
 *   1 | 1 0 0 0 0
 *   2 | 0 0 0 0 0
 *   3 | 0 0 0 0 1
 *   4 | 0 0 0 1 1
 *     -----------
 *       0 1 2 3 4
 */


        gameOfLife.seed({
                                {0, 0},
                                {1, 0},
                                {0, 1},

                                {4, 3},
                                {3, 4},
                                {4, 4},
                        });

        gameOfLife.tick();
        REQUIRE(containsOnly(gameOfLife, {
                {1, 1},
                {3, 3},
        }));

/*
 *   0 | 0 0 0 0 0
 *   1 | 0 1 0 0 0
 *   2 | 0 0 0 0 0
 *   3 | 0 0 0 1 0
 *   4 | 0 0 0 0 0
 *     -----------
 *       0 1 2 3 4
 */


    }

}
