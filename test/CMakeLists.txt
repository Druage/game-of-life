cmake_minimum_required(VERSION 3.16)
project(game-of-life-test)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} GameOfLifeTest.cpp include/catch.hpp include/utils.h)

target_link_libraries(${PROJECT_NAME} GameOfLifeLib)