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

// Constants definitions
#define ARG_PROGNAME 0
#define ARG_INPUT_FILE 1
#define ARG_OUTPUT_FILE 2
#define ARG_GEN_COUNT 3
#define ARG_COUNT 4

// Error codes definitions
#define ERR_FILE_NONEXIST -1        // File doesn't exist
#define ERR_ALLOC -2                // Allocation error
#define ERR_REALLOC -3              // Reallocation error
#define ERR_ASYMMETRIC -4           // Input table isn't symmetric
#define ERR_FORBIDDEN_CHAR -5       // In input file is forbidden character
#define ERR_TABLE_TOO_LARGE -6      // Input table is too large
#define ERR_WRITE -7                // Csv write eror

// Functions definitions
bool **read_csv(char *filename, uint8_t *row_count, uint8_t *col_count);                                  // Read csv file with initial conditions and create array
uint8_t get_num_neighbours(bool **field, uint8_t row, uint8_t col, uint8_t row_count, uint8_t col_count); // Get the number of alive neighbour cells
bool **update_board(bool **field, uint8_t row_count, uint8_t col_count);                                  // Calculate the next generation
void print_board(bool **field, uint8_t row_count, uint8_t col_count);                                     // Print board
bool write_csv(bool **field, char *filename, uint8_t row_count, uint8_t col_count);                       // Write game board into a csv file

int main(int argc, char *argv[]) {
// On Windows UTF-8 isn't default charset. We need it
#ifdef _WIN32
    system("CHCP 65001");
#endif

    if(argc != ARG_COUNT) {
        printf("Usage: game_of_life [INPUT FILE] [OUTPUT FILE] [LAST GENERATION]\n");
    } else {
        char *aux; // Auxiliary pointer for strtoul
        uint32_t last_gen = strtoul(argv[ARG_GEN_COUNT], &aux, 10); // Get num of generations to calculate
        uint8_t row_num, col_num; // Size of board

        printf("Welcome to Conway's Game of Life\n");
        printf("Opening file\t[");
        bool **board = read_csv(argv[ARG_INPUT_FILE], &row_num, &col_num); // Get initial conditions from csv
        printf("ok]\n");
        printf("0. generation:\n");
        print_board(board, row_num, col_num);

        // Calculate next generations
        for(register uint32_t i = 1; i <= last_gen; ++i) {
            printf("%d. generation\n", i);
            board = update_board(board, row_num, col_num);
            print_board(board, row_num, col_num);
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

bool **read_csv(char *filename, uint8_t *row_count, uint8_t *col_count) {
    FILE *input;                // Input file
    bool **brd;                 // Output pointer to pointer array
    int16_t int_buf;            // Buffer for fgetc
    uint8_t col_num_ctrl = 1;   // Auxiliary variable to detect different row lengths
    *col_count = 1;
    *row_count = 1;

    if((input = fopen(filename, "r")) == NULL) {          // Open input file
        printf("ERR]\n");
        exit(ERR_FILE_NONEXIST);
    }
    if((brd = (bool**) malloc(sizeof(bool*))) == NULL) {  // Pointer to pointer array initial allocation
        printf("ERR]\n");
        exit(ERR_ALLOC);
    }
    if((brd[0] = (bool*) malloc(sizeof(bool))) == NULL) { // -//-
        printf("ERR]\n");
        exit(ERR_ALLOC);
    }

    // Csv parsing
    while(true) {
        int_buf = (int16_t) fgetc(input);

        // If char is 0 or 1, insert it into array
        if(((char) int_buf == '0') || ((char) int_buf == '1')) {
            brd[*row_count - 1][*col_count - 1] = int_buf - '0';
        // If char is column separator, add new column to row
        } else if(((char) int_buf == ';') || ((char) int_buf == ',')) {     // Allowed column separators
            if(*col_count != 255) {
                ++*col_count;
            } else {
                printf("ERR]\n");
                exit(ERR_TABLE_TOO_LARGE);
            }

            if((brd[*row_count - 1] = (bool*) realloc(brd[*row_count - 1], sizeof(bool) * (*col_count))) == NULL) {
                printf("ERR]\n");
                exit(ERR_REALLOC);
            }

            if(*row_count == 1)
                ++col_num_ctrl;
        // If char is new line, allocate new row
        } else if((char) int_buf == '\n') {
            if(*row_count != 255) {
                ++*row_count;
            } else {
                printf("ERR]\n");
                exit(ERR_TABLE_TOO_LARGE);
            }

            if((brd = (bool**) realloc(brd, sizeof(bool*) * (*row_count))) == NULL) {
                printf("ERR]\n");
                exit(ERR_REALLOC);
            }

            if((brd[*row_count - 1] = (bool*) malloc(sizeof(bool))) == NULL) {
                printf("ERR]\n");
                exit(ERR_ALLOC);
            }

            if((*row_count > 2) && (col_num_ctrl != (*col_count))) {
                printf("ERR]\n");
                exit(ERR_ASYMMETRIC);
            }

            *col_count = 1;
        // EOF - brake loop
        } else if(int_buf == EOF) {
            break;
        // In file is undefined character
        } else {
            printf("ERR]\n");
            exit(ERR_FORBIDDEN_CHAR);
        }
    }

    *col_count = col_num_ctrl;
    --*row_count;
    if(*col_count != *row_count) { // Test if the number of rows corresponds to the number of columns
        printf("ERR]\n");
        exit(ERR_ASYMMETRIC);
    }

    fclose(input);
    return brd;
}

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

void print_board(bool **field, uint8_t row_count, uint8_t col_count) {
    for(register uint8_t i = 0; i < row_count; ++i) {
        for(register uint8_t j = 0; j < col_count; ++j) {
            if(field[i][j])
                printf("\u2588"); // full block
            else
                printf("\u2591"); // light shade
        }
        printf("\n");
    }
    printf("\n");
}

bool write_csv(bool **field, char *filename, uint8_t row_count, uint8_t col_count) {
    FILE *output;

    if((output = fopen(filename, "w")) == NULL) {
        printf("ERR]\n");
        exit(ERR_WRITE);
    }

    for(register uint8_t i = 0; i < row_count; ++i) {
        for(register uint8_t j = 0; j < col_count; ++j) {
            if(j == col_count - 1) {
                fprintf(output, "%d", field[i][j]);
            } else {
                fprintf(output, "%d;", field[i][j]);
            }
        }
        if(i != row_count - 1)
            fprintf(output, "\n");
    }

    fclose(output);
}
