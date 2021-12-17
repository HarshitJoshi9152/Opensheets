#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "types.h"
#include "functions.h"

void freeSheet(SheetObject sheet)
{
    free(*sheet.headers);

    for (int r = 0; r < sheet.rowsCount; ++r)
    {
        for (int c = 0; c < sheet.colsCount; ++c)
        {
            free(sheet.grid[r][c]);
        }
    }
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

char *evaluateCell(char *cell)
{
    int len_cell = strlen(cell);
    char buffer[SIZE] = {0};
    int bufferCounter = 0;

    for (int i = 0; i < len_cell; ++i)
    {
        // i is the offset. the current character added position.
        buffer[bufferCounter++] = cell[i];

        Function bufferMatch = findFunctionMatch(buffer);

        switch (bufferMatch)
        {
            case NO_MATCH:
                // add the expressions on the left and right
                continue;

            case DATE:
            {
                char *time = date();
                time[strlen(time) - 1] =  '\x00'; //coz we dont want the newline at the end lmao.

                char *args = getArgsString(cell, i);

                // rewriteCellAt(cell, from, till, new_text);
                rewriteCell(cell, i - 4 + 1, i + strlen(args), time);   //`date` = 4
                free(args); // should we free the text in rewriteCell ?
                break;
            }

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
SheetObject parseSheet(char *data)
{

    SheetObject sheet = {0};

    // parsing the headers? to get the on of columns ig
    char firstline[SIZE] = "";
    sscanf(data, "%50[^\n]%*c", firstline); // reading the first line

    char *colHeader = strtok(firstline, ",");

    do
    {
        sheet.headers[sheet.colsCount] = malloc(strlen(colHeader) + 1);
        strcpy(sheet.headers[sheet.colsCount++], colHeader);
    } while ((colHeader = strtok(NULL, ",")));

    // parsing the rest of the rows

    char *line;
    int lineCount = 0;

    line = strtok(data, "\n");

    // reading the rest of the lines.
    while ((line = strtok(0, "\n")))
    {

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