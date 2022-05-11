cmake_minimum_required(VERSION 3.23)
project(game_of_life C)

set(CMAKE_C_STANDARD 99)

add_executable(game_of_life main.c gol_constants.h gol_csv.c gol_csv.h gol_game.c gol_game.h)
