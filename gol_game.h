// Game functions for Game of Life project

#ifndef GAME_OF_LIFE_GOL_GAME_H
#define GAME_OF_LIFE_GOL_GAME_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "gol_constants.h"

uint8_t get_num_neighbours(bool **field, uint8_t row, uint8_t col, uint8_t row_count, uint8_t col_count); // Get the number of alive neighbour cells
bool **update_board(bool **field, uint8_t row_count, uint8_t col_count);                                  // Calculate the next generation
void print_board(bool **field, uint8_t row_count, uint8_t col_count, uint8_t print_type);                 // Print board

#endif //GAME_OF_LIFE_GOL_GAME_H
