cmake_minimum_required(VERSION 3.16)
project(GameOfLifeLib)

set(CMAKE_CXX_STANDARD 20)

add_library(${PROJECT_NAME} STATIC ../lib/GameOfLife_T.cpp ../lib/GameOfLife_T.h)

target_include_directories(${PROJECT_NAME} PUBLIC .)