#ifndef _FUNCTION_H
#define _FUNCTION_H

#include <stdlib.h>
#include "vector3.h"
#include "figure.h"

typedef struct functionCDT * Function;

typedef enum{
    PARAMETER_STRING,
    PARAMETER_INT,
    PARAMETER_FLOAT,
    PARAMETER_VECTOR3,
    PARAMETER_VECTOR3INT,
    PARAMETER_BOOLEAN,
    PARAMETER_FIGURE
} ParameterType;

typedef union {
    char * string;
    int integer;
    float floating;
    Vector3 vector;
    Vector3Int vectorInt;
    bool boolean;
    Figure figure;
} ParameterValue;

Function newFunction(char * name, void (*function_ptr), size_t paramCount, ...);

char * getFunctionName(Function function);

size_t getParameterCount(Function function);

ParameterType * getExpectedParameters(Function function);

int executeFunction(Function function, ParameterValue * parameterValueArray) ;

void freeFunction(Function function);

#endif //_FUNCTION_H
