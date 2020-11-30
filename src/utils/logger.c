#include "logger.h"
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <stdbool.h>

#define U3D_NAME "u3d"
#define LOG_LOCATION "u3dc.log"
#define CLEAR_LOG true

void initDebugLog(){
    const char * openMethod = CLEAR_LOG ? "w+" : "a+"; 

    FILE * file = fopen(LOG_LOCATION, openMethod);

    time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    fprintf(file, "---------------------------%02d/%02d/%04d %02d:%02d:%02d---------------------------\n", 
    timeinfo->tm_mday, timeinfo->tm_mon, timeinfo->tm_year+1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

    fclose(file);
}

void logDebug(char * format, ...){
    #ifdef DEBUG
    FILE * file = fopen(LOG_LOCATION, "a+");
    va_list args;
    va_start(args, format);
    fprintf(file, "DEBUG: "); 
    vfprintf(file, format, args);  
    fclose(file);
    #endif
}

void logInfo(char * format, ...){
    FILE * file = fopen(LOG_LOCATION, "a+");
    va_list args;
    va_start(args, format);
    vfprintf(file, format, args);  
    fclose(file);
}

void logError(ErrorType type, char * format, ...){
    va_list args;
    va_start(args, format);
    fprintf(stderr, "%s: ", U3D_NAME);
    fprintf(stderr, "\033[1;31m");
    switch (type)
    {
    case FATAL_ERROR:
        fprintf(stderr, "fatal error: ");
        break;
    case SYNTAX_ERROR:
        fprintf(stderr, "syntax error: ");
        break;
    case ERROR:
    default:
        fprintf(stderr, "error: ");
        break;
    }
    
    fprintf(stderr, "\033[0m");
    vfprintf(stderr, format, args);  
}

void logWarning(char * format, ...){
    va_list args;
    va_start(args, format);
    fprintf(stderr, "%s: ", U3D_NAME);
    fprintf(stderr, "\033[0;33m");
    fprintf(stderr, "warning: ");
    fprintf(stderr, "\033[0m");
    vfprintf(stderr, format, args);  
}