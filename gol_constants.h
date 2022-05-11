// Constants for Game of Life project

#ifndef GAME_OF_LIFE_GOL_CONSTANTS_H
#define GAME_OF_LIFE_GOL_CONSTANTS_H

// Constants definitions
#define ARG_PROGNAME 0
#define ARG_INPUT_FILE 1
#define ARG_OUTPUT_FILE 2
#define ARG_GEN_COUNT 3
#define ARG_PRINTTYPE 4
#define ARG_COUNT 5

// Error codes definitions
#define ERR_FILE_NONEXIST -1        // File doesn't exist
#define ERR_ALLOC -2                // Allocation error
#define ERR_REALLOC -3              // Reallocation error
#define ERR_ASYMMETRIC -4           // Input table isn't symmetric
#define ERR_FORBIDDEN_CHAR -5       // In input file is forbidden character
#define ERR_TABLE_TOO_LARGE -6      // Input table is too large
#define ERR_WRITE -7                // Csv write eror
#define ERR_UNDEFINED_PRINTING -8   // Undefined printing format

#endif //GAME_OF_LIFE_GOL_CONSTANTS_H
