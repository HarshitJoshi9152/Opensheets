#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <errno.h>

#define ARGCOUNTREQUIRED 2

void exit_program(char *error_string)
{
    fprintf(stderr, error_string);
    exit(EXIT_FAILURE);
}

/** void count_occurences(char *string, char **str_dict) */
/** { */
/**     int count = 0; */

/**     return count; */
/** } */

void prints(char *str, ...) {

    // adding newline to string 
    char *str_buffer = malloc(strlen(str) + 1);
    strcat(str_buffer, str);
    strcat(str_buffer, "\n");

    //printf("\n%p\n", str_buffer);

    va_list args;
    va_start(args, str);
    vfprintf(stdout, str_buffer, args);
    va_end(args);
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

int main(int argc, char *argv[])
{
    if (argc < ARGCOUNTREQUIRED ) exit_program("Not enough arguments\n");

    // printing arguments
    //printf("%d\n", argc);
    //printf("%s\n", argv[0]);
    //printf("%s\n", argv[1]);
    //printf("%s\n", argv[2]);

    const char *input_file = argv[1];
    //const char *output_file = argv[2];

    if (access(input_file, R_OK) != 0) exit_program("Input File Inaccessible\n");

    //if (excel_file_fd == NULL) exit_program("Input File Inaccessible\n");
    //
    //// reading from the file
    //
    //// finding size of file
    //fseek(excel_file_fd, 0L, SEEK_END);
    //long int f_size = ftell(excel_file_fd);
    //rewind(excel_file_fd);
    ////fseek(excel_file_fd, 0L, SEEK_START);
    //printf("\n%ld\n", f_size);
    //
    //char *file_content = malloc(f_size);
    ////fscanf(excel_file_fd, "%s", file_content); // it skips \n newline ?? wait can this only read one line ! shit
    //                                           // scanf("%100[^\n]%*c",buffer); what would this do ?
    //
    //// fgets(file_content, f_size, excel_file_fd);
    //int rt = fgets(file_content, f_size, excel_file_fd);
    //while ((rt)) {
    //    rt = fgets(file_content+strlen(file_content), f_size, excel_file_fd);
    //}
    //// closing the file
    //fclose(excel_file_fd);

    char *file_content = readFromFile(input_file);
    printf("file_content = \n%s", file_content);

    // printf("%s", file_content);
    //prints("nic%de", 10 , 20);

    // okay so start with reading the file
    //FILE *ptr = fopen("fileopen","mode");
    return 0;
}
 


