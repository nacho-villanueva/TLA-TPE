#ifndef _FUNCTION_H
#define _FUNCTION_H

#include <stdlib.h>
#include "vector3.h"
#include "figure.h"

typedef struct functionCDT * Function;

typedef struct functionOverloadCDT * FunctionOverload;

typedef enum{
    PARAMETER_STRING,
    PARAMETER_INT,
    PARAMETER_FLOAT,
    PARAMETER_VECTOR3,
    PARAMETER_VECTOR3INT,
    PARAMETER_BOOLEAN,
    PARAMETER_FIGURE,
    PARAMETER_VARIABLE
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

Function newFunction(char * name);

void addFunctionOverload(Function function, int (*pFunction)(size_t, ParameterValue[], ParameterType[]), size_t paramCount, ...);

char * getFunctionName(Function function);

size_t getFunctionOverloadCount(Function function);

FunctionOverload * getFunctionOverloads(Function function);

size_t getParameterCount(FunctionOverload overload);

ParameterType * getExpectedParameters(FunctionOverload overload);

int executeFunction(FunctionOverload overload, size_t paramCount, ParameterValue * parameterValueArray, ParameterType * type) ;

void freeFunction(Function function);

#endif //_FUNCTION_H
