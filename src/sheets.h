#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define SIZE 50
#define MAX_FUNCTION_NAME_LEN 4
#define MIN_FUNCTION_NAME_LEN 3

typedef enum {false, true} bool;
typedef struct sheetobject
{
    long long rowsCount;
    long long colsCount;
    char *headers[SIZE]; // col headers
    char *grid[SIZE][SIZE]; // rows that would contains cells (columns)
} SheetObject;

typedef enum {
    NO_MATCH,
    SUM,
    SUB,
    MUL,
    DIV,
    DATE,
} Function;

const char* FunctionStrings[] = {
    [NO_MATCH] = "NO_MATCH",
    [SUM] = "SUM",
    [SUB] = "SUB",
    [MUL] = "MUL",
    [DIV] = "DIV",
    [DATE] = "DATE",
};

void freeSheet(SheetObject sheet) {
    free(*sheet.headers);
    free(**sheet.grid);
}

void printCell(SheetObject sheet, int r, int c)
{
    printf("%s\n", sheet.grid[r][c]);
}

// todo: implement table with border.
void printSheet(SheetObject sheet)
{
    printf("    "); // padding 4 spaces
    // printing the headers
    for (int i = 0; i < sheet.colsCount; ++i)
        printf("%s, ", sheet.headers[i]);
    printf("\n");

    // printing the rows
    for (int r = 0; r < sheet.rowsCount; ++r)
    {
        printf("%d   ", r); // row numbering
        for (int c = 0; c < sheet.colsCount; ++c)
        {
            printf("%s, ", sheet.grid[r][c]);
        }
        printf("\n");
    }
}

Function findFunctionMatch(const char *bufferString)
{
    /*
    sub the MAX_FUNCTION_NAME_LEN from the strlen to get the offset
    from which to check if the rest of the string matches any function names

       0123456789
    ex cellValue3SUM            offset = strlen(cellValue3SUM) - MAX_FUNCTION_NAME_LEN
                ^                      = 13 - 4 = 9
                |               end = 13 - MIN_FUNCTION_NAME_LEN = 13 - 3 = 10

    the code will try to match "3SUM" to all the function but no match will be found.
    so the offset will be incremented and the new resulting string "SUM" will be checked
    for match against all the functions, when a match is found the match index is returned.
    */

    int len = strlen(bufferString);
    int offset = len - MAX_FUNCTION_NAME_LEN;
    int end = len - MIN_FUNCTION_NAME_LEN;
    int func_count = sizeof(FunctionStrings)/sizeof(FunctionStrings[0]);

    if (offset < 0) return NO_MATCH; // we dont want a negative index

    for (offset; offset <= end; ++offset)
    {
        for (int f_i = 0; f_i < func_count; f_i++)
        {
            int match = !strcmp(bufferString + offset, FunctionStrings[f_i]);
            if (match) return f_i;
            // i can just return f_i because enums get reduced to ints anyways right !
        }
    }
    return NO_MATCH;
}

char* evaluateCell(char* cell) {
    int len_cell = strlen(cell);
    char buffer[SIZE] = {0};
    int bufferCounter = 0;

    for (int i=0; i < len_cell; ++i)
    {
        buffer[bufferCounter++] = cell[i];

        Function bufferMatch = findFunctionMatch(buffer);

        switch (bufferMatch)
        {
            case NO_MATCH:
                // add the expressions on the left and right
                continue;

            default:
                printf("<bufferMatch %s>\n", FunctionStrings[bufferMatch]);
                break;
        }
    }
    return cell;
}

SheetObject evaluateSheet(SheetObject sheet)
{
    // https://blog.logrocket.com/learn-these-keyboard-shortcuts-to-become-a-vs-code-ninja/
    for (int r = 0; r < sheet.rowsCount; ++r)
    {
        for (int c = 0; c < sheet.colsCount; ++c)
        {
            char *cell = sheet.grid[r][c];
            evaluateCell(cell);
        }
    }
    return sheet;
}

// this MUTATES THE CHAR* DATA 
SheetObject parseSheet(char *data) {

    SheetObject sheet = {0};

    // parsing the headers? to get the on of columns ig
    char firstline[SIZE] = "";
    sscanf(data, "%50[^\n]%*c", firstline); // reading the first line

    char *colHeader = strtok(firstline, ",");

    do {
        sheet.headers[sheet.colsCount] = malloc(strlen(colHeader) + 1);
        strcpy(sheet.headers[sheet.colsCount++], colHeader);
    }
    while((colHeader = strtok(NULL, ",")));

    // parsing the rest of the rows

    char *line;
    int lineCount = 0;

    line = strtok(data, "\n"); 

    // reading the rest of the lines.
    while((line = strtok(0, "\n"))) {

        // looping over line to find the separate cells 
        char *cells[SIZE] = {0};
        char buffer[SIZE] = {0}; // this is how long a cell can be.
        int bufferlen = 0;
        int cellsFound = 0;

        for (int charCount = 0; charCount < strlen(line); charCount++)
        {
            char letter = line[charCount];
            if (letter != ',')
                buffer[bufferlen++] = letter;

            if (letter == ',' || charCount == strlen(line) - 1) // coz [1,2,3] elms = 3 ,(s) = 2
            {
                // add to list of cells
                cells[cellsFound] = malloc(bufferlen + 1); // for the null character
                strcpy(cells[cellsFound++], buffer);
                // reset buffer
                bufferlen = 0;
                memset(buffer, 0, SIZE);
            }
        }

        assert(cellsFound == sheet.colsCount && "UNEQUAL CELLS");
        // can we add the cell to sheet struct directly ??? ig we can lol
        memcpy(sheet.grid[lineCount], cells, cellsFound * sizeof(cells[0]));
        lineCount++;
    }

    sheet.rowsCount = lineCount;

    return sheet;
}