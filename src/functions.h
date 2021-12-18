#include <string.h>
#include <time.h>
#include <math.h>

#include "types.h"

#define MAX_FUNCTION_NAME_LEN 4
#define MIN_FUNCTION_NAME_LEN 3
#define DELIMITER " "
#define ARGS_LIMIT 10

char* sum(char *args);
char* sub(char *args);
char* mul(char *args);
char* divide(char *args);
char* date();

Function findFunctionMatch(const char *bufferString);

void rewriteCell(char* cell, int from, int till, char* new_text)
{

    int cell_len = strlen(cell);
    int new_text_len = strlen(new_text);

    char* buffer = malloc(cell_len - (till - from + 1) + strlen(new_text) + 1); // +1 for the NULL character
    strncpy(buffer, cell, from);
    strcpy(buffer+from, new_text);
    strcpy(buffer+from+new_text_len, cell+till+1);


    void* allocated = realloc(cell, cell_len - (till - from + 1) + strlen(new_text) + 1);
    if (!allocated) exit_program("Could Not Realloc enough memory !");

    strcpy(cell, buffer);
    free(buffer);
    // but we will have to free each cell too if its malloc allocated, nope all cells are already malloc allocated;
}

char* getArgsString(char *cell, int start_index) {
    // `index` is the index where the function name ends so parentheses should start at index + 1
    assert(cell[start_index+1] == '(' &&  "Invalid Function call, No Parentheses"); // not a function call | invalid function call;

    // determining the len of the args
    int args_len = 0;
    for (int i = start_index + 1; i < strlen(cell); ++i)
    {
        args_len++;
        if (cell[i] == ')') break;
    }

    assert(cell[args_len + args_len] != ')' && "Invalid Function call, No Closing Parentheses");

    // copying the args part to new string.
    char *args = malloc(args_len + 1);
    strncpy(args, (char *)(cell + start_index + 1), args_len);
    args[args_len + 1] = '\x00';

    return args;
}

int* parseArgsAsInts(char* args, int *args_count)
{
    args[strlen(args) - 1] = '\x00';  // the last char is ')' and that wont be parsed by `atoi` function

    int *argv = calloc(ARGS_LIMIT, sizeof(int));
    memset(argv, 0, ARGS_LIMIT * sizeof(int));

    int argv_count = 0; // could we remove this variable because args_count would always be 0 ?

    char* arg = strtok(args, DELIMITER);
    argv[argv_count++] = atoi(arg+1); // arg + 1 becase the arg char is `(` and the last is `)`;

    while((arg = strtok(NULL, DELIMITER)))
    {
        int num = atoi(arg);
        if (num == 0) exit_program("Could Not Parse argument as Number");
        // argv[*(args_count)++] = num; num); // why does this not wwork ?
        argv[argv_count++] = num;
    }

    *args_count = argv_count;
    return argv;
}


char* sum(char *args)
{
    int args_count = 0;
    int* argv = parseArgsAsInts(args, &args_count);

    int s = 0;
    for (int i = 0; i < args_count; i++) {
        s += argv[i];
    }
    free(argv);

    char *str = malloc(10);
    sprintf(str, "%d", s);

    return str;
}

// char* sub(char *args)
// {
//     int args_count = 0;
//     int* argv = parseArgsAsInts(args, &args_count);
//     printf("<args_count = %d>", args_count);
//     free(argv);
// }

// char* mul(char *args)
// {
//     int args_count = 0;
//     int* argv = parseArgsAsInts(args, &args_count);
//     printf("<args_count = %d>", args_count);
//     free(argv);
// }

// char* divide(char *args)
// {
//     int args_count = 0;
//     int* argv = parseArgsAsInts(args, &args_count);
//     printf("<args_count = %d>", args_count);
//     free(argv);
// }

char* date()
{
    time_t t = time(NULL);
    struct tm *time = localtime(&t);
    return asctime(time);
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

    for (; offset <= end; ++offset)
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
