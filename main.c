/* Game of life
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

// Constants definitions
#define ARG_PROGNAME 0
#define ARG_INPUT_FILE 1
#define ARG_OUTPUT_FILE 1
#define ARG_GEN_COUNT 3

// Error codes definitions
#define ERR_FILE_NONEXIST 1         // File doesn't exist
#define ERR_ALLOC 2                 // Allocation error
#define ERR_REALLOC 3               // Reallocation error
#define ERR_ASYMMETRIC 4             // Input table isn't symmetric
#define ERR_FORBIDDEN_CHAR 5        // In input file is forbidden character

// Global variables definition
uint8_t row_num = 1, col_num = 1;   //Size of field

// Functions definitions
bool **read_csv(char *filename);
uint8_t get_num_neighbours(bool **field, uint8_t row, uint8_t col);
bool **update_field();
uint8_t write_csv(bool **field, char *filename);

int main(int argc, char *argv[]) {
    bool **board = read_csv("input.csv");

    for(register uint8_t i = 0; i < row_num; ++i) {
        for(register uint8_t j = 0; j < col_num; ++j) {
            printf("%d ", board[i][j]);
        }
        printf("\n");
    }

    for(register uint8_t i = 0; i < row_num; ++i) {
        free(board[i]);
        board[i] = NULL;
    }
    free(board);
    board = NULL;
    return 0;
}

bool **read_csv(char *filename) {
    FILE *input;
    bool **brd;
    int16_t int_buf;
    uint8_t col_num_ctrl = 1;

    if((input = fopen(filename, "r")) == NULL)
        exit(ERR_FILE_NONEXIST);
    if((brd = (bool**) malloc(sizeof(bool*))) == NULL)
        exit(ERR_ALLOC);
    if((brd[0] = (bool*) malloc(sizeof(bool))) == NULL)
        exit(ERR_ALLOC);

    while(true) {
        int_buf = (int16_t) fgetc(input);

        if(((char) int_buf == '0') || ((char) int_buf == '1')) {
            brd[row_num - 1][col_num - 1] = int_buf - '0';
        } else if(((char) int_buf == ';') || ((char) int_buf == ',')) { // Allowed column separators
            ++col_num;

            if((brd[row_num - 1] = (bool*) realloc(brd[row_num - 1], sizeof(bool) * col_num)) == NULL)
                exit(ERR_REALLOC);

            if(row_num == 1)
                ++col_num_ctrl;
        } else if((char) int_buf == '\n') {
            ++row_num;

            if((brd = (bool**) realloc(brd, sizeof(bool*) * row_num)) == NULL)
                exit(ERR_REALLOC);

            if((brd[row_num - 1] = (bool*) malloc(sizeof(bool))) == NULL)
                exit(ERR_ALLOC);

            if((row_num > 2) && (col_num_ctrl != col_num)) {
                exit(ERR_ASYMMETRIC);
            }

            col_num = 1;
        } else if(int_buf == EOF) {
            break;
        } else {
            exit(ERR_FORBIDDEN_CHAR);
        }
    }

    col_num = col_num_ctrl;
    --row_num;
    if(col_num != row_num) {
        exit(ERR_ASYMMETRIC);
    }

    fclose(input);
    return brd;
}