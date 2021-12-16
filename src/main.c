
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <assert.h>

#include "./helper.h"

#define ARGCOUNTREQUIRED 2
#define SIZE 50
typedef struct sheetobject
{
    long long rowsCount;
    long long colsCount;
    char *headers[SIZE]; // col headers
    char *grid[SIZE][SIZE]; // rows that would contains cells (columns)
} SheetObject;

// this MUTATES THE CHAR* DATA 
SheetObject parseSheet(char *data) {

    SheetObject sheet = {0};

    // parsing the headers? to get the on of columns ig
    char firstline[SIZE];
    sscanf(data, "%SIZE[^\n]%*c", firstline); // reading the first line

    char *colHeader = strtok(firstline, ",");

    do {
        sheet.headers[sheet.colsCount] = malloc(strlen(colHeader) + 1);
        strcpy(sheet.headers[sheet.colsCount++], colHeader);
    }
    while(colHeader = strtok(NULL, ","));

    // parsing the rest of the rows

    char *line;
    char *lines[SIZE];
    int lineCount = 0;

    line = strtok(data, "\n"); 

    // lines[lineCount] = malloc(strlen(line) + 1); // we dont want the first line its the header !
    // strcpy(lines[lineCount++], line);

    // reading the rest of the lines.
    while(line = strtok(0, "\n")) {
        lines[lineCount] = malloc(strlen(line) + 1); // + 1 for the null character ig ?

        // looping over line to find the separate cells 
        char *cells[SIZE];

        char buffer[SIZE]; // this is how long a cell can be.
        int bufferlen = 0;
        int cellsFound = 0;

        for (int charCount = 0; charCount < strlen(line); charCount++) {
            // printf("charCount = <%d>\n", charCount);
            char letter = line[charCount];
            if (letter == ',' || charCount == strlen(line) - 1) {       // coz [1,2,3] elms = 3 ,(s) = 2
                // add to list
                cells[cellsFound] = malloc(bufferlen + 1); // for the null character
                strcpy(cells[cellsFound++], buffer);
                // reset buffer
                bufferlen = 0;
                memset(0, buffer, bufferlen);
            } else {
                buffer[bufferlen++] = letter;
            }
        }

        assert(cellsFound == sheet.colsCount);
        // strcpy(lines[lineCount++], line);
        // sheet.grid[lineCount] = cells;
        memcpy(sheet.grid[lineCount], cells, cellsFound * sizeof(cells[0]));
        lineCount++;
    }

    sheet.rowsCount = lineCount;

    return sheet;
}

// okay it doesnt work !
// char **splitLine(char *line, char *delim) {
//     char **parts; // array of character arrays ? strings array ?
//     int partCount = 0;

//     char *part = strtok(line, delim);

//     do {
//         parts[partCount] = malloc(strlen(part) + 1);
//         strcpy(parts[partCount++], part);
//     }
//     while(part = strtok(NULL, ","));

//     return parts;
// }

// ya this got too tough
SheetObject sparseSheet(char *data)
{
    // okay so read the file line by line.
    char line[SIZE];
    char *lines[SIZE];
    long long lineCount = 0;
    // read dcharacter by charaacter and increment llines on newline !
    long long charCount = 0;
    while(sscanf(data + charCount, "%s", line) != EOF) {
        lines[lineCount] = malloc(strlen(line));
        strcpy(lines[lineCount++], line);
        charCount += strlen(line) + 1;
        printf("%s\n", line);
    }
    free(*lines);
}

int main(int argc, char *argv[])
{
    if (argc < ARGCOUNTREQUIRED ) exit_program("Not enough arguments\n");

    const char *input_file = argv[1];
    //const char *output_file = argv[2];

    if (access(input_file, R_OK) != 0) exit_program("Input File Inaccessible\n");

    char *file_content = readFromFile(input_file); // we need to free it since its malloc allocated
    //printf("file_content = \n%s", file_content);
    SheetObject sheet = parseSheet(file_content);
    printf("%s\n", sheet.grid[1][2]);

    free(file_content);
    // so the file should be a csv right ?
    return 0;
}
 


