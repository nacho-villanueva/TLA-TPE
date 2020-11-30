#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "logger.h"
#include "../configs.h"

char outputFile[MAX_PATH_SIZE] = "output.pde";

int initParser(char * filename){
    strcpy(outputFile, filename);
    FILE * file = fopen(outputFile, "w+");
    if(file == NULL){
        logError(ERROR, "parser.c: Could not open file,\n");
        return -1;
    }

    fclose(file);
    return 0;
}

void parse(char * format, ...){
    FILE * file = fopen(outputFile, "a+");
    va_list args;
    va_start(args, format);
    vfprintf(file, format, args);  
    fclose(file);
}