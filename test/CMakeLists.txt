cmake_minimum_required(VERSION 3.16)
project(game-of-life)

set(CMAKE_CXX_STANDARD 20)

add_executable(game-of-life GameOfLifeTest.cpp catch.hpp ../src/GameOfLife_T.cpp ../src/GameOfLife_T.h utils/utils.h)
