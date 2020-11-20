#ifndef _LOGGER_H_
#define _LOGGER_H_

typedef enum {
    ERROR,
    FATAL_ERROR,
    SYNTAX_ERROR
} ErrorType;

void initDebugLog();

void logDebug(char * format, ...); 

void logInfo(char * format, ...);

void logError(ErrorType type, char * format, ...);

void logWarning(char * format, ...);

#endif //_LOGGER_H_