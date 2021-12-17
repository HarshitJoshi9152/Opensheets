#pragma once

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
