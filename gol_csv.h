// CSV functions for Game of Life project

#ifndef GAME_OF_LIFE_GOL_CSV_H
#define GAME_OF_LIFE_GOL_CSV_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include "gol_constants.h"

bool **read_csv(char *filename, uint8_t *row_count, uint8_t *col_count);            // Read csv file with initial conditions and create array
bool write_csv(bool **field, char *filename, uint8_t row_count, uint8_t col_count); // Write game board into a csv file

#endif //GAME_OF_LIFE_GOL_CSV_H
