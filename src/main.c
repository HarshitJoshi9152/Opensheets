
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <errno.h>

#include "./helper.h"

#define ARGCOUNTREQUIRED 2

int main(int argc, char *argv[])
{
    if (argc < ARGCOUNTREQUIRED ) exit_program("Not enough arguments\n");

    const char *input_file = argv[1];
    //const char *output_file = argv[2];

    if (access(input_file, R_OK) != 0) exit_program("Input File Inaccessible\n");

    char *file_content = readFromFile(input_file);
    printf("file_content = \n%s", file_content);

    return 0;
}
 


