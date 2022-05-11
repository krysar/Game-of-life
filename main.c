/* Game of Life
 * Created as project for PC2M subject
 * Author: Filip Poloček (VUT ID: 240875)
 * Created: 10.5.2022
 * Last edit: 10.5.2022
 */

// Header files includes
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <malloc.h>

#include "gol_constants.h"
#include "gol_csv.h"
#include "gol_game.h"

int main(int argc, char *argv[]) {
// On Windows UTF-8 isn't default charset. We need it
#ifdef _WIN32
    system("CHCP 65001");
#endif

    if(argc != ARG_COUNT) {
        printf("Usage: %s [INPUT FILE] [OUTPUT FILE] [LAST GENERATION] [PRINTING TYPE]\n", argv[ARG_PROGNAME]);
    } else {
        char *aux; // Auxiliary pointer for strtoul
        uint32_t last_gen = strtoul(argv[ARG_GEN_COUNT], &aux, 10); // Get num of generations to calculate
        uint8_t printing_type = (uint8_t) strtoul(argv[ARG_PRINTTYPE], &aux, 10);
        uint8_t row_num, col_num; // Size of board

        printf("Welcome to Conway's Game of Life\n");
        printf("Opening file\t[");
        bool **board = read_csv(argv[ARG_INPUT_FILE], &row_num, &col_num); // Get initial conditions from csv
        printf("ok]\n");
        printf("0. generation:\n");
        print_board(board, row_num, col_num, printing_type);

        // Calculate next generations
        for(register uint32_t i = 1; i <= last_gen; ++i) {
            printf("%d. generation\n", i);
            board = update_board(board, row_num, col_num);
            print_board(board, row_num, col_num, printing_type);
        }

        printf("Saving game board to %s\t[", argv[ARG_OUTPUT_FILE]);
        write_csv(board, argv[ARG_OUTPUT_FILE], row_num, col_num);
        printf("ok]\nExiting\n");

        for(register uint8_t i = 0; i < row_num; ++i) {
            free(board[i]);
            board[i] = NULL;
        }
        free(board);
        board = NULL;
    }

    return 0;
}
