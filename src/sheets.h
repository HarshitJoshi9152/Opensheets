#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define SIZE 50

typedef enum {false, true} bool;
typedef struct sheetobject
{
    long long rowsCount;
    long long colsCount;
    char *headers[SIZE]; // col headers
    char *grid[SIZE][SIZE]; // rows that would contains cells (columns)
} SheetObject;


void freeSheet(SheetObject sheet) {
    free(*sheet.headers);
    free(**sheet.grid);
}

void printCell(SheetObject sheet, int r, int c)
{
    printf("%s\n", sheet.grid[r][c]);
}

void printSheet(SheetObject sheet)
{
    for (int r = 0; r < sheet.rowsCount; ++r)
    {
        for (int c = 0; c < sheet.colsCount; ++c)
        {
            printf("\"%s\" ", sheet.grid[r][c]);
        }
        printf("\n");
    }
}

// this MUTATES THE CHAR* DATA 
SheetObject parseSheet(char *data) {

    SheetObject sheet = {0};

    // parsing the headers? to get the on of columns ig
    char firstline[SIZE];
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

        assert(cellsFound == sheet.colsCount);
        // can we add the cell to sheet struct directly ??? ig we can lol
        memcpy(sheet.grid[lineCount], cells, cellsFound * sizeof(cells[0]));
        lineCount++;
    }

    sheet.rowsCount = lineCount;

    return sheet;
}