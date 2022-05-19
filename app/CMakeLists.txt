cmake_minimum_required(VERSION 3.16)
project(GameOfLife)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL2 REQUIRED)

add_executable(${PROJECT_NAME} main.cpp ../lib/GameOfLife_T.cpp ../lib/GameOfLife_T.h include/seeds.h include/colors.h)

if (SDL2_FOUND)
    message(STATUS "SDL2 Found, Linking...")
    include_directories(${SDL2_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} GameOfLifeLib ${SDL2_LIBRARIES})
else()
    message(FATAL_ERROR "SDL2 Not Found")
endif()



