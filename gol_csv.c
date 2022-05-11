// CSV functions for Game of Life project

#include "gol_csv.h"

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
