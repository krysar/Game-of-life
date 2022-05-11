// Game functions for Game of Life project

#include "gol_game.h"

uint8_t get_num_neighbours(bool **field, uint8_t row, uint8_t col, uint8_t row_count, uint8_t col_count) { // I think this function is relatively clear to understand
    uint8_t num = 0;

    for(register int16_t i = row - 1; i <= row + 1; ++i) {
        if(i < 0) {
            continue;
        } else if(i > row_count - 1) {
            break;
        } else {
            for(register int16_t j = col - 1; j <= col + 1; ++j) {
                if(j < 0) {
                    continue;
                } else if(j > col_count - 1) {
                    continue;
                } else {
                    if(j == col) {
                        if(i == row)
                            continue;
                    }
                    if(field[i][j])
                        ++num;
                }
            }
        }
    }

    return num;
}

bool **update_board(bool **field, uint8_t row_count, uint8_t col_count) {
    bool **updated; // Array for new generation

    if((updated = (bool**) malloc(sizeof(bool*) * row_count)) == NULL) {
        exit(ERR_ALLOC);
    }

    for(register uint8_t i = 0; i < row_count; ++i) {
        if((updated[i] = (bool*) malloc(sizeof(bool) * col_count)) == NULL) {
            exit(ERR_ALLOC);
        }
    }

    // Calculate new generation of game board based on S23/B3 rules
    for(register uint8_t i = 0; i < row_count; ++i) {
        for(register uint8_t j = 0; j < col_count; ++j) {
            uint8_t neighbours = get_num_neighbours(field, i, j, row_count, col_count);

            if(neighbours < 2) {
                updated[i][j] = false;          // Any live cell with fewer than two live neighbours dies
            } else if(neighbours == 2) {
                updated[i][j] = field[i][j];    // Any live cell with two live neighbours stay alive/died
            } else if(neighbours == 3) {
                updated[i][j] = true;           // Any dead cell with exactly three live neighbours becomes a live cell
            } else {
                updated[i][j] = false;          // Any live cell with more than three live neighbours dies
            }
        }
    }

    return updated;
}

void print_board(bool **field, uint8_t row_count, uint8_t col_count, uint8_t print_type) {
#ifdef _WIN32
    if(print_type == 1)
        system("cls");
#elif defined(__unix) || defined(__unix__) || defined(linux) || defined(__linux) || defined(__linux__)
    if(print_type == 1)
        system("clear");
#endif
    for(register uint8_t i = 0; i < row_count; ++i) {
        for(register uint8_t j = 0; j < col_count; ++j) {
            if(field[i][j])
                printf("\u2588\u2588"); // full block
            else
                printf("\u2591\u2591"); // light shade
        }
        printf("\n");
    }
    printf("\n");
}
