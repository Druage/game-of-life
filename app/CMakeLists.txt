cmake_minimum_required(VERSION 3.16)
project(app)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} main.cpp ../lib/GameOfLife_T.cpp ../lib/GameOfLife_T.h)

target_link_libraries(${PROJECT_NAME} game-of-life-lib)