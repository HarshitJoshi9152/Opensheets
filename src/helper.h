#pragma once

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

// print a msg to stderr stream and exit with EXIT_FAILURE
void exit_program(char *error_string)
{
    fputs(error_string, stderr);
    exit(EXIT_FAILURE);
}

long getFileSize(FILE *fd)
{
    fseek(fd, 0, SEEK_END);
    long size = ftell(fd);
    rewind(fd);
    return size;
}

char* readFromFile(const char *filename)
{
    FILE *fd = fopen(filename, "r");

    if (fd == NULL) {
        errno = 1;
        exit_program("Input File Inaccessible\n");
    }

    long int size = getFileSize(fd);
    char *contents = malloc(size);

    char c;
    while((c = fgetc(fd)) != EOF) {
        //contents[ctr++] = c;
        *(contents++) = c;
    }

    fclose(fd);
    return contents - size;

    /* fscanf(buffer, "%s", file_descriptor); only works to read one line*/
}


void prints(char *str, ...) {

    // adding newline to string 
    char *str_buffer = malloc(strlen(str) + 1);
    strcat(str_buffer, str);
    strcat(str_buffer, "\n");

    va_list args;
    va_start(args, str);
    vfprintf(stdout, str_buffer, args);
    va_end(args);
}
