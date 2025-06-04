cmake_minimum_required(VERSION 3.10)
project(TicTacToe)

set(CMAKE_CXX_STANDARD 17)

option(USE_STATIC_LIB "Use static library" ON)

include_directories(include)

if(USE_STATIC_LIB)
    add_library(game_logic STATIC src/game_logic.cpp)
else()
    add_library(game_logic SHARED src/game_logic.cpp)
endif()

add_executable(TicTacToe main.cpp)
target_link_libraries(TicTacToe game_logic)
