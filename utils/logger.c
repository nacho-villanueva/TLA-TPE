#include "logger.h"
#include <stdio.h>
#include <stdarg.h>


void logDebug(char * format, ...){
    #ifdef DEBUG
    va_list args;
    va_start(args, format);
    vprintf(format, args);  
    #endif

}