#ifndef _LOGGER_H_
#define _LOGGER_H_

void initDebugLog();

void logDebug(char * format, ...); 

void logInfo(char * format, ...);

void logError(char * formate, ...);

#endif //_LOGGER_H_